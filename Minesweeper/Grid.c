#include <SFML/Graphics.h>

#include "Grid.h"

Cell* CellCreate(sfVector2f size, sfVector2f pos, sfColor color) 
{
	// Initialize all cell properties
	// ...
	
	// Use sfText_create() to create a Text and contains it in a sfText pointer
	sfText* newText = sfText_create();
	// Create its font with sfFont_createFromFile(fontFilePath) contains it in a sfFont pointer, 
	// where fontFilePath is the path to find the .ttf file in your project
	sfFont* newFont = sfFont_createFromFile("C:/Users/paulc/Documents/projets vscode/c/CHA_pau_Jeu2D/Minesweeper/Resources/Roboto-Regular.ttf");
	if (!newFont) // Check if it found the font file to be sure
		return NULL;

	// Use sfText_setString(newText, text) to set the text you want, 
	// where newText is your sfText pointer and text is "your text" (char[] or const char*)
	sfText_setString(newText, " ");
	// Assign the font to your sfText with sfText_setFont(newText, newFont), 
	// where newText is your sfText pointer and newFont is your sfFont pointer
	sfText_setFont(newText, newFont);
	// Finally set the size of your text with sfText_setCharacterSize(newText, size),
	// where newText is your sfText pointer and size is a integer (int)
	sfText_setCharacterSize(newText, (int)size.x);
	sfText_setPosition(newText, pos);

	// Use sfRectangleShape_create() to create a Rectangle and contains it in a sfRectangleShape pointer
	sfRectangleShape* newShape = sfRectangleShape_create();
	// Set its size with sfRectangleShape_setSize(newShape, size),
	// where newShape is your sfRectangleShape pointer and size is a sfVector2f
	sfRectangleShape_setSize(newShape, size);
	// Set its position with sfRectangleShape_setPosition(newShape, position),
	// where newShape is your sfRectangleShape pointer and position is a sfVector2f
	sfRectangleShape_setPosition(newShape, pos);
	// Finally set its color with sfRectangleShape_setFillColor(newShape, color),
	// where newShape is your sfRectangleShape pointer and color is a sfColor
	sfRectangleShape_setFillColor(newShape, color);

	Cell* newCell = (Cell*)malloc(sizeof(Cell));
	newCell->shape = newShape;
	newCell->text = newText;
	newCell->bDiscovered = false;
	newCell->bFlagged = false;
	newCell->bPlanted = false;
	newCell->explosiveNeighbor = 0;

	newCell->size = size;
	newCell->pos = pos;
	newCell->color = color;

	return newCell;
}

void CellDraw(Cell* cell, sfRenderWindow* window)
{
	// Draw the cell shape and text on the window 
	if (cell->shape) {
		sfRenderWindow_drawText(window, cell->text, NULL);
		sfRenderWindow_drawRectangleShape(window, cell->shape, NULL);
	}
}

int CellReveal(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	// If the cell is already discovered or flagged, do nothing and return 0
	if (cell->bDiscovered == true || cell->bFlagged == true) {
		return 0;
	}

	// If the cell is planted with a bomb, return FAILURE to indicate game over
	if (cell->bPlanted == true) {
		return FAILURE;
	}
	
	// Change the cell's appearance to revealed (lighter color) and mark it as discovered
	cell->bDiscovered = true;
	sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGBA(0, 255, 0, 100));

	// If the cell has explosive neighbors, display the number
	if (cell->explosiveNeighbor > 0) {
		switch (cell->explosiveNeighbor) {
		case 1:
			sfText_setString(cell->text, "1");
			break;
		case 2:
			sfText_setString(cell->text, "2");
			break;
		case 3:
			sfText_setString(cell->text, "3");
			break;
		case 4:
			sfText_setString(cell->text, "4");
			break;
		case 5:
			sfText_setString(cell->text, "5");
			break;
		case 6:
			sfText_setString(cell->text, "6");
			break;
		case 7:
			sfText_setString(cell->text, "7");
			break;
		case 8:
			sfText_setString(cell->text, "8");
			break;
		default:
			break;
		}
		
	}

	// If the cell is completely empty (explosiveNeighbor == 0), start the "flood fill" (reveal neighbors) algorithm
	else if (cell->explosiveNeighbor == 0) {
		// reveal les 8 adjacentes si existent
		if (cellGridPos.x > 0) {
			CellReveal(grid, (sfVector2i){cellGridPos.x - 1, cellGridPos.y });
		}
		if (cellGridPos.y > 0) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x , cellGridPos.y-1 });
		}
		if (cellGridPos.x < GRID_SIZE-1) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x + 1, cellGridPos.y });
		}
		if (cellGridPos.y < GRID_SIZE-1) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x , cellGridPos.y + 1 });
		}
		if (cellGridPos.x > 0 && cellGridPos.y > 0) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x - 1, cellGridPos.y - 1 });
		}
		if (cellGridPos.x < GRID_SIZE - 1 && cellGridPos.y > 0) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x + 1, cellGridPos.y - 1 });
		}
		if (cellGridPos.x > 0 && cellGridPos.y < GRID_SIZE - 1) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x - 1, cellGridPos.y + 1 });
		}
		if (cellGridPos.x < GRID_SIZE - 1 && cellGridPos.y < GRID_SIZE - 1) {
			CellReveal(grid, (sfVector2i) { cellGridPos.x + 1, cellGridPos.y + 1 });
		}
	}

	// Increase grid discovered cell count and If all none planted cells are discovered, terminate the game (return SUCCESS)
	grid->discoveredCellCount += 1;
	if (grid->discoveredCellCount == (GRID_SIZE*GRID_SIZE - BOMB_COUNT)) {
		return SUCCESS;
	}

	// Return 0 as the cell was revealed and was not a bomb
	return 0;
}

void CellFlag(Grid* grid, sfVector2i cellGridPos)
{
	Cell* cell = grid->cells[cellGridPos.x][cellGridPos.y];

	// If the cell is already discovered, do nothing and return
	if (cell->bDiscovered == true) {
		return;
	}

	// Toggle the flagged state of the cell and update its appearance accordingly
	if (cell->bFlagged == false)
	{
		sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGBA(255, 10, 10, 255));
		cell->bFlagged = true;
	}
	else {
		sfRectangleShape_setFillColor(cell->shape, sfColor_fromRGBA(255, 255, 255, 255));
		cell->bFlagged = false;
	}
	return;
}

void CellDestroy(Cell* cell)
{
	// Free all resources associated with the cell
	sfRectangleShape_destroy(cell->shape);
	sfText_destroy(cell->text);
	free(cell);
}

Grid* GridCreate()
{
	Grid* newGrid = (Grid*)malloc(sizeof(Grid));
	
	// Initialize grid properties, create all cell and register them in grid array
	// ...
	sfVector2f size = { CELL_SIZE,CELL_SIZE };
	//Il faut faire le calcul de l'offset pour la pos
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			sfVector2f pos = { i*CELL_SIZE+i*CELL_OFFSET+GRID_OFFSET,j*CELL_SIZE+j*CELL_OFFSET+GRID_OFFSET };
			Cell* newCell = CellCreate(size, pos, sfColor_fromRGBA(255, 255, 255, 255));
			newGrid->cells[i][j] = newCell;
		}
	}
	newGrid->discoveredCellCount = 0;

	return newGrid;
}

void GridPlantBomb(Grid* grid, int bombCount, sfVector2i cellToAvoid)
{
	// Plant all bomb and avoid avoided spot
	// Update explosiveNeighbor count for all cells around each bomb planted
	for (int i = 0; i < bombCount; i++) {
		Cell* bomb_cell;
		int colonne;
		int ligne;
		do {
			colonne = rand() % GRID_SIZE;
			ligne = rand() % GRID_SIZE;
			bomb_cell = grid->cells[colonne][ligne];
		} while (bomb_cell->bPlanted == true 
			|| (colonne == cellToAvoid.x && ligne == cellToAvoid.y)
			|| (colonne == cellToAvoid.x+1 && ligne == cellToAvoid.y)
			|| (colonne == cellToAvoid.x - 1 && ligne == cellToAvoid.y)
			|| (colonne == cellToAvoid.x && ligne == cellToAvoid.y+1)
			|| (colonne == cellToAvoid.x + 1 && ligne == cellToAvoid.y+1)
			|| (colonne == cellToAvoid.x - 1 && ligne == cellToAvoid.y+1)
			|| (colonne == cellToAvoid.x && ligne == cellToAvoid.y-1)
			|| (colonne == cellToAvoid.x + 1 && ligne == cellToAvoid.y-1)
			|| (colonne == cellToAvoid.x - 1 && ligne == cellToAvoid.y-1)
			);
		bomb_cell->bPlanted = true;
	}

	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			Cell* cell_modif = grid->cells[i][j];
			if (i > 0) {
				if (grid->cells[i - 1][j]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (j > 0) {
				if (grid->cells[i][j-1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (i < GRID_SIZE - 1) {
				if (grid->cells[i + 1][j]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (j < GRID_SIZE - 1) {
				if (grid->cells[i][j+1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (i > 0 && j > 0) {
				if (grid->cells[i - 1][j-1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (i < GRID_SIZE - 1 && j > 0) {
				if (grid->cells[i + 1][j-1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (i > 0 && j < GRID_SIZE - 1) {
				if (grid->cells[i - 1][j+1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
			if (i < GRID_SIZE - 1 && j < GRID_SIZE - 1) {
				if (grid->cells[i + 1][j+1]->bPlanted == true) {
					cell_modif->explosiveNeighbor += 1;
				}
			}
		}
	}
}

sfVector2i GridUpdateLoop(Grid* grid, sfRenderWindow* window)
{
	// Get mouse position relative to the window
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);



	/*
	sfVector2i cellCoord;
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			Cell* currentCell = grid->cells[i][j];
			if (IsContained(mousePos, currentCell->shape)) {
				cellCoord = (sfVector2i){ currentCell->pos.x ,currentCell->pos.y };
				return cellCoord;
			}
		}
	}
	*/




	// Initialize hovered cell coordinates to (-1, -1) (no cell hovered)
	sfVector2i cellCoord = { (mousePos.x / CELL_SIZE) - 1, (mousePos.y / CELL_SIZE) - 1 };
	// Search for hovered cell (if any)
	// Return cell coordinates or (-1, -1) if no cell is hovered
	// Use global bounds and contains function from SFML to detect if mouse is over a cell
	if (cellCoord.x >= 0 && cellCoord.x < GRID_SIZE && cellCoord.y >= 0 && cellCoord.y < GRID_SIZE) {
		return cellCoord;
	}

	return (sfVector2i) { -1, -1 };
}

void GridDraw(Grid* grid, sfRenderWindow* window)
{
	// Draw all cells in the grid
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			CellDraw(grid->cells[i][j],window);
		}
	}
}

void GridDestroy(Grid* grid)
{
	// Free all resources associated with the grid and its cells
	for (int i = 0; i < GRID_SIZE; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			if (grid->cells[i][j] !=NULL)
			{
				CellDestroy(grid->cells[i][j]);
			}
		}
	}
	free(grid);
}
