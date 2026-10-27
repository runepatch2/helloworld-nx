// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

void cls(void) {
    printf("\33[2J");
}

void printControls(void)
{
    printf("----- CONTROLS -----\n");
    printf("Press 'L' to add 'Hello, World!' to an output.txt file.\n");
    printf("Press 'R' to print current output.txt contents.\n");
    printf("Press '-' to erase the contents of output.txt.\n");
    printf("Press '+' to exit and return to hbmenu.\n");
    printf("Press 'X' to clear screen and print these controls.\n");
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("Goodbye, Mars!\n");
    printf("And Hello, World!\n");
    printControls();

    FILE* fp;
    fp = fopen("output.txt", "a+");
    if (fp == NULL) {
        perror("fileopen broke lol.\n");
        return 1;
    }

    int count = 1;
    char s[1024];

    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

        // Your code goes here

        if (kDown & HidNpadButton_L) {
            fprintf(fp, "\n%d | Hello, World!", count);
            count++;
        }

        if (kDown & HidNpadButton_R) {
            rewind(fp);
            printf("\n-----------------");
            while (fgets(s, sizeof(s), fp) != NULL) {
                printf(s);
            }
        }

        if (kDown & HidNpadButton_Minus) {
            freopen("output.txt", "w+", fp);
            count = 1;
        }

        if (kDown & HidNpadButton_X) {
            cls();
            printControls();
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}