'''
A simple make program for c and c++ programs. 

Determines files to build with from a make file
and compiles them with gcc. 

To see an example make file run
```
python3 bear_make.py -eg
```

After making a make file for you code, run this to
create an executable
```
python3 bear_make.py my_make_file
```


'''



import argparse
import hashlib
import os
import re
import shutil
import subprocess
import time

example = '''

# this is a comment
"""
this is also a comment

Every .c, .cpp, or .o file in a provided directory will be included in the 
'gcc -o' command. Bear Make automatically detects changes in files 
from run to run, avoiding compiling unchanged files. These compiled
files are kept in the 'build' folder created when you run Bear Make. 

If the 'build' folder isn't in the current directory (deleted or moved)
Bear Make will start over and make a clean

When added files by name, you'll only need to include source or object files, not 
header files (.hpp or .h)

"""

EXECUTABLE_NAME='some_name'

DIRECTORY='some_dir_path'
FILE='some_file_path'


'''


ANSII_RESET = "\033[0m"
def print_in_color(str: str, rgb_color_code: str) -> str:
    return ''.join([rgb_color_code, str, ANSII_RESET])

def red(str: str) -> str:
    return print_in_color(str, '\033[38;2;255;0;0m')

def blue(str: str) -> str:
    return print_in_color(str, '\033[38;2;0;7;212m')

def green(str: str) -> str:
    return print_in_color(str, '\033[38;2;40;133;0m')

def yellow(str: str) -> str:
    return print_in_color(str, '\033[38;2;214;191;39m')

def extract_from_quotes(str: str) -> str:
    index = str.index("'")
    content = str[index+1:]
    
    i = len(content)-1
    while i > 0:
        if content[i] == "'":
            break
        i-=1
    content = content[:i]

    return content

def get_files_recursively(directory: str) -> list[str]:
    
    files = []
    for filename in os.listdir(directory):
        full_path = os.path.join(directory, filename)
        if os.path.isfile(full_path):
            files.append(full_path)
        elif os.path.isdir(full_path):
            rec_files = get_files_recursively(full_path)
            files.extend(rec_files)

    return files

def hash_file_blake2b(file_path):
    hash_obj = hashlib.blake2b()
    with open(file_path, 'rb') as f:
        while chunk := f.read(8192):
            hash_obj.update(chunk)
    return hash_obj.hexdigest()


parser = argparse.ArgumentParser(description="A make program similar to cmake but more simple (hopefully)")
parser.add_argument('file', nargs='?', help='path to your make file')
parser.add_argument('-eg', '--example', required=False, action='store_true', help='add this flag to output an example make file')
parser.add_argument('-no', '--no_optimization', required=False, action='store_true', help='instead of keeping object files to speed up builds')
parser.add_argument('-r', '--release', required=False, action='store_true', help='runs in release mode. otherwise optimization is off and the executable is built for debugging')
args = parser.parse_args()

if args.example:
    print(example)
else:
    
    start_time = time.time()
    with open(args.file, 'r') as file:
        

        name = 'my_exe'
        files = []
        headers = []
        object_files = []
        flags = ''


        directories = []
        in_comment = False
        try:
            for line in file:

                if not line.startswith('#'):
                    if line.startswith('"""') or line.startswith("'''"):
                        in_comment = not in_comment

                    if not in_comment:
                        if line.startswith('EXECUTABLE_NAME'):
                            name = line.split('=')[1]
                            name = extract_from_quotes(name)
                        elif line.startswith('DIRECTORY'):
                            directory = line.split('=')[1]
                            directory = extract_from_quotes(directory)
                            directories.append(directory)
                        elif line.startswith('FILE'):
                            file_path = line.split('=')[1]
                            file_path = extract_from_quotes(file_path)
                            files.append(file_path)
                        elif line.startswith('FLAGS'):
                            flags = line.split('=')[1]
                            flags = extract_from_quotes(flags)

                                

        except Exception as e:
            print("Error on line: ")
            print("\t" + red((str(e))))
            exit(1)


    try:
        # all files
        for directory in directories:
            files_in_directory = get_files_recursively(directory)
            for file in files_in_directory:
                # .h .c, .cpp, .hpp or .o
                if file.endswith('.c') or file.endswith('.cpp'):
                    files.append(file)
                elif file.endswith('.h') or file.endswith('.hpp'):
                    headers.append(file)
                elif file.endswith('.o'):
                    object_files.append(file)

        command = ['gcc']
        if not args.release:
            command += ['-g', '-O0', '-Wall', '-Wextra']
        
        command.append('-o')
        command.append(name)

        if args.no_optimization:
            command.append(files)
        else:
            # load hashes
            hashes = {}
            if os.path.exists('build/hashes'):
                with open('build/hashes', 'r') as file:
                    for line in file:
                        if '=' in line:
                            file_path, hash = line.split('=')
                            hashes[file_path] = hash[:-1]


            # object files
            for file in object_files:
                copy_path = 'build/' + file
                os.makedirs(os.path.dirname(copy_path), exist_ok=True)
                shutil.copy2(file, copy_path)
                command.append(file)


            # compile if changed or new
            files_to_recompile = []
            for file in files:
                new_path, ext = os.path.splitext('build/' + file)
                new_path += '.o'
                recompile = not os.path.exists(new_path) or file not in hashes
                if not recompile:
                    # recompile if file has changed
                    if file in hashes:
                        old_hash = hashes[file]
                        hash = hash_file_blake2b(file)
                        if old_hash != hash: 
                            recompile = True

                    # recompile if dependencies have changes
                    if not recompile:
                        result = subprocess.run(['gcc', '-MM', file], capture_output=True, text=True)
                        dependencies_str = result.stdout.replace('\\', '').replace('\n', '')
                        dependencies_str = re.sub(r'\s+', ' ', dependencies_str)
                        dependencies = dependencies_str.split(' ')
                        if len(dependencies) > 2:
                            dependencies = dependencies[2:]
                            for dependency in dependencies:
                                dep_path = os.path.normpath(dependency)

                                dep_hash = hash_file_blake2b(dep_path)
                                if dep_path not in hashes or hashes[dep_path] != dep_hash:
                                    hashes[dep_path] = dep_hash
                                    recompile = True


                    

                # recompile if needed
                if recompile:
                    print(blue('COMPILING / RECOMPILING ') + file)
                    compile_command = ["gcc", "-c"]
                    if not args.release:
                        compile_command += ['-g', '-O0']
                    compile_command.append(file)
                    print(green(' '.join(compile_command)))
                    result = subprocess.run(compile_command, capture_output=True, text=True)
                    if result.returncode != 0:
                        raise Exception(result.stderr)
                    if result.stderr != '':
                        print(yellow(result.stderr))
                    file_name = os.path.basename(file)
                    root, ext = os.path.splitext(file_name)
                    object_file = root + '.o'
                    
                    os.makedirs(os.path.dirname(new_path), exist_ok=True)
                    shutil.move(object_file, new_path)

                    # add hash
                    hash = hash_file_blake2b(file)
                    if file in hashes:
                        old_hash = hashes[file]
                        print('file changed: (' + file + ')' + " " + old_hash + " -> " + hash)
                    else:
                        print('new file: (' + file + ') ' + hash)

                    hashes[file] = hash

                command.append(new_path)


            # write hashes
            with open('build/hashes', 'w') as file:
                for file_path, hash in hashes.items():
                    file.write(file_path + "=" + hash)
                    file.write('\n')
            

        # add user extra args
        if len(flags) > 0:
            command.append(flags)


        # call gcc
        print(blue('CREATING PROGRAM'))
        print(green(' '.join(command)))
        result = subprocess.run(command, capture_output=True, text=True)
        if result.returncode != 0:
            raise Exception(result.stderr)
        print(result)

        total_time = time.time() - start_time
        print(
            blue("{:.2f}".format(total_time) + 's')
        )



    except Exception as e:
        print(red((str(e))))
        exit(1)