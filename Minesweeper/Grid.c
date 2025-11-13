#include <SFML/Graphics.h>

#include "Grid.h"

Cell* CellCreate(sfVector2f size, sfVector2f pos, sfColor color) 
{
	// Initialize all cell properties
	// ...
	
	Cell* newCell = (Cell*)malloc(sizeof(Cell));
	return newCell;
}

void CellDraw(Cell* cell, sfRenderWindow* window)
{
	// Draw the cell shape and text on the window
	// ...
}

int CellReveal(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	// If the cell is already discovered or flagged, do nothing and return 0
	// ...

	// If the cell is planted with a bomb, return FAILURE to indicate game over
	// ...
	
	// Change the cell's appearance to revealed (lighter color) and mark it as discovered
	// ...

	// If the cell has explosive neighbors, display the number
	// ...

	// If the cell is completely empty (explosiveNeighbor == 0), start the "flood fill" (reveal neighbors) algorithm
	// ...

	// Increase grid discovered cell count and If all none planted cells are discovered, terminate the game (return SUCCESS)
	// ...

	// Return 0 as the cell was revealed and was not a bomb
	return 0;
}

void CellFlag(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	// If the cell is already discovered, do nothing and return
	// ...


	// Toggle the flagged state of the cell and update its appearance accordingly
	// ...
}

void CellDestroy(Cell* cell)
{
	// Free all resources associated with the cell
	// ...
}

Grid* GridCreate()
{
	Grid* newGrid = (Grid*)malloc(sizeof(Grid));
	
	// Initialize grid properties, create all cell and register them in grid array
	// ...
	
	return newGrid;
}

void GridPlantBomb(Grid* grid, int bombCount, sfVector2i cellToAvoid)
{
	// Plant all bomb and avoid avoided spot
	// Update explosiveNeighbor count for all cells around each bomb planted
	// ...
}

sfVector2i GridUpdateLoop(Grid* grid, sfRenderWindow* window) 
{
	// Get mouse position relative to the window
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);

	// Initialize hovered cell coordinates to (-1, -1) (no cell hovered)
	sfVector2i cellCoord = { -1, -1 };

	// Search for hovered cell (if any)
	// Return cell coordinates or (-1, -1) if no cell is hovered
	// Use global bounds and contains function from SFML to detect if mouse is over a cell
	// ...
	
	return cellCoord;
}

void GridDraw(Grid* grid, sfRenderWindow* window)
{
	// Draw all cells in the grid
	// ...
}

void GridDestroy(Grid* grid)
{
	// Free all resources associated with the grid and its cells
	// ...
}
