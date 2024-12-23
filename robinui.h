#ifndef IUPUI_H_INCLUDED
#define IUPUI_H_INCLUDED

Ihandle *inputGrid();
Ihandle *timeQuantumInput();
int setProcessNum(Ihandle *self);
Ihandle *processNumInput();
int getCellValue(Ihandle *grid, int rowNum, int colNum);
bool invalidTimeQuantum(int TQ);

#endif // IUPUI_H_INCLUDED
