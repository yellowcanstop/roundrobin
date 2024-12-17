#include <stddef.h>
#include <stdbool.h>
#include <iup.h>
#include <stdio.h>
#include <stdlib.h>
#include "robinui.h"

/* Initializes the input grid */
Ihandle *inputGrid() {
    Ihandle *gbox;
    gbox = IupGridBox
    (
      IupSetAttributes(IupLabel("PID"), "FONTSTYLE=Bold"),
      IupSetAttributes(IupLabel("Arrival Time"), "FONTSTYLE=Bold"),
      IupSetAttributes(IupLabel("Burst Time"), "FONTSTYLE=Bold"),
      IupSetAttributes(IupLabel("I/O Time"), "FONTSTYLE=Bold"),
      NULL
    );
    IupSetAttribute(gbox, "NAME", "GRID");
    for(int i = 1; i < 11; i++){
        char index[4];
        sprintf(index, "%d", i);
        Ihandle *pid = IupSetAttributes(IupLabel(index), "ALIGNMENT=ACENTER, VISIBLE=FALSE");
        Ihandle *arrivalInput = IupSetAttributes(IupText("0"), "FILTER=NUMBER, PADDING=3x3, VISIBLE=FALSE");
        Ihandle *burstInput = IupSetAttributes(IupText("0"), "FILTER=NUMBER, PADDING=3x3, VISIBLE=FALSE");
        Ihandle *ioInput = IupSetAttributes(IupText("0"), "FILTER=NUMBER, PADDING=3x3, VISIBLE=FALSE");
        IupAppend(gbox, pid);
        IupAppend(gbox, arrivalInput);
        IupAppend(gbox, burstInput);
        IupAppend(gbox, ioInput);
    }
    IupRefresh(gbox);
    IupSetAttribute(gbox, "EXPANDCHILDREN", "HORIZONTAL");
    IupSetAttribute(gbox, "NUMDIV", "4");
    IupSetAttribute(gbox, "ALIGNMENTLIN", "ACENTER");
    IupSetAttribute(gbox, "MARGIN", "10x10");
    IupSetAttribute(gbox, "GAPLIN", "5");
    IupSetAttribute(gbox, "GAPCOL", "5");
    return gbox;
}

/* Initializes time quantum input box */
Ihandle *timeQuantumInput() {
    Ihandle *timeQuantum;
    timeQuantum = IupHbox
    (
      IupSetAttributes(IupLabel("Enter time quantum (ms):"), "FONTSTYLE=Bold"),
      IupSetAttributes(IupText("Arrival Time"), "NAME=TQINPUT, FILTER=NUMBER, SIZE=20x10, MARGIN=3x3"),
      IupSetAttributes(IupLabel("(Max 100)"), "FONTSTYLE=Bold"),
      NULL
    );
    return timeQuantum;
}

/* setProcessNum activates when the Set Button is pressed
   Could be used to set number of rows in the grid */
int setProcessNum(Ihandle *self){
    Ihandle* gridBox = IupGetDialogChild(self, "GRID");
    Ihandle* processNum = IupGetDialogChild(self, "processNum");
    char* value = IupGetAttribute(processNum, "VALUE");
    int showRowTo = 4*atoi(value);
    int hideRowFrom = showRowTo + 4;
    for(int i = 4; i < 4+showRowTo; i++){
        Ihandle* cell = IupGetChild(gridBox, i);
        IupShow(cell);
    }
    for(int i=hideRowFrom; i < 44 ; i++){
        Ihandle* cell = IupGetChild(gridBox, i);
        IupHide(cell);
    }
    IupRefreshChildren(gridBox);
    return IUP_DEFAULT;
}

/* Initializes Process Num dropdown */
Ihandle *processNumInput() {
    Ihandle *processNum;
    processNum = IupHbox
    (
     IupSetAttributes(IupLabel("Enter number of processes:"), "FONTSTYLE=Bold"),
     IupSetAttributes(IupList(NULL), "NAME=processNum, DROPDOWN=YES, 1=1,2=2,3=3,4=4,5=5,6=6,7=7,8=8,9=9,10=10"),
     IupSetAttributes(IupButton("Set", NULL), "FONTSTYLE=Bold, PADDING=3x3"),
     NULL
    );
    return processNum;
}

/* getCellValue is adapted for the Process input grid exactly
   Used to create Processes*/
int getCellValue(Ihandle *grid, int rowNum, int colNum){
    int cellPos = rowNum*4 + colNum; // *4 skips the header row, +colNum gets the cell in the row
    Ihandle *cell = IupGetChild(grid, cellPos);
    char* valueStr = IupGetAttribute(cell, "VALUE");
    int value = atoi(valueStr);
    return value;
}

bool invalidTimeQuantum(int TQ){
    if(TQ > 100 || TQ < 1){
        return true;
    }else{
        return false;
    }
}


