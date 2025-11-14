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
    Grid* grid = GridCreate();

    sfVector2i currentCell = { -1,-1 };
    

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
            if (event.type == sfEvtMouseMoved) {
                currentCell = GridUpdateLoop(grid, window);
            }

            if (event.type == sfEvtMouseButtonPressed) {
                if (event.key.code == sfMouseLeft) {
                    if (currentCell.x >= 0 && &currentCell.y >= 0) {
                        if (bFirstTouch) {
                            bFirstTouch = false;
                            GridPlantBomb(grid, BOMB_COUNT, currentCell);
                        }
                        switch (CellReveal(grid, currentCell)) {
                            case FAILURE:
                                printf("YOU LOSE !!");
                                return FAILURE;
                            case SUCCESS:
                                printf("YOU WIN !!");
                                return SUCCESS;
                            default:
                                break;
                        }
                    }
                }
                if (event.key.code == sfMouseRight) {
                    if (currentCell.x >= 0 && currentCell.y >= 0) {
                        CellFlag(grid, currentCell);
                    }
                }
            }
        }

        
        /* Clear the screen */
        sfRenderWindow_clear(window, sfBlack);
        
        // Draw everything here
        GridDraw(grid, window);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */
    GridDestroy(grid);
    sfRenderWindow_destroy(window);

    return SUCCESS;
}
