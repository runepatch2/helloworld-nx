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

int validateFileOpen(FILE *gfp)
{
    if (gfp == NULL) {
        perror("fopen broke lol.\n");
        return 1;
    }
    return 0;
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

    int count;
    char s[1024];

    FILE* fp;
    FILE* cfp;
    fp = fopen("output.txt", "a+");
    validateFileOpen(fp);

    // must open file as read first before doing checks!!!
    cfp = fopen("count.txt", "a+");

    validateFileOpen(cfp);
    cfp = freopen("count.txt", "r+", cfp);
    (cfp = freopen("count.txt", "r+", cfp));
    validateFileOpen(cfp);
    // then check if it's empty
    bool countEmpty = ((fgetc(cfp)) == EOF);
    // if it is empty, set the count to 1 and write that into the file
    if (countEmpty) {
        freopen("count.txt", "w+", cfp);
        count = 1;
        fputc(count, cfp);
    }
    
    else {
        freopen("count.txt", "a+", cfp);
        count = fgetc(cfp);
    }

    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus) {
            fclose(fp);
            fclose(cfp);
            break; // break in order to return to hbmenu
        }

        if (kDown & HidNpadButton_L) {
            fprintf(fp, "\n%d | Hello, World!", count);
            count++;
            freopen("count.txt", "w+", cfp);
            freopen("count.txt", "a+", cfp);
            fputc(count, cfp);
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
            freopen("count.txt", "w+", cfp);
            putc(1, cfp);
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