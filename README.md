# terminal images

A termainl app to display images in the terminal (for terminals that support ansii rgb colors but don't allow changing pixels in the terminal).

Really this is basically an ascii art generator that also has colors. 

There is also a single header file in this repo at [dependencies/TerminalImages.h](dependencies/TerminalImages.h) that contains the logic to read
an image and return a array of unicode characters with corresponding ansii color codes. This could be used in other apps for converting images
into ascii art by copying the file.

# Compiling
We use a python script to compile this program, which is included in the repo. It relies on gcc so it will probably only work on Linux, Mac, and WSL.
You can compile it like so:
```
python3 bear_make.py make
```

This generates a gcc command which is run to compile the program. With the current dependencies (inluded in the repo) the command looks like this:
```
gcc -g -O0 -Wall -Wextra -o ti build/main.o -lm
```

But that could change


# Usage

Here's how the command can be used
```
./ti path/to/your/image.png 
```

Which takes an image like this:

![wizards](test/wizards.jpg)

And outputs this in the terminal:

![terminal output](test/example_screenshot.png)


So it's not pixel per pixel (as most terminals don't support that) but is good for getting the gist of an image.

The image will appear with more quality as you increase the terminal dimensions. For many terminals, descreasing the font size is the way to do this.

This only works if your terminal support “ANSI true color” or “24-bit color escape codes”

Does work:
- GNOME terminals
- iterm2
- xterm
- tmux
- windows terminal 10+

Doesn't work;
- windows cmd
- mac terminal
