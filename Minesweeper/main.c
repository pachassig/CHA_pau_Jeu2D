#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include "basics.h"
#include "Grid.h"

#define DEBUG_CLEAN false

int main(void)
{
    // Define the video mode and create the window
    sfVideoMode mode = { WIDTH, HEIGHT, 32 };
    sfRenderWindow* window;
    sfEvent event; // Event variable to handle events

    /* Create the main window */
    window = sfRenderWindow_create(mode, "Minesweeper", sfClose, NULL);
    if (!window)
    {
            return NULL_WINDOW;
    }

    // Seed the random number generator (to use rand() later)
    srand((unsigned int)time(NULL));

    // Initialize the game grid and all you need to set up before starting the game loop (Creating Grid, Planting bombs, etc.)
    // ...

    printf("Start Game ! \n");
    bool bFirstTouch = true;

    /* Start the game loop */
    while (sfRenderWindow_isOpen(window))
    {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            /* Close window : exit */
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
            }

            // Handle all events here
            // ...
        }

        
        /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);
        
        // Draw everything here
        // ...

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    // ...
    sfRenderWindow_destroy(window);

    return SUCCESS;
}
