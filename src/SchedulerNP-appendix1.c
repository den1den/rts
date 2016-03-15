(...)
//uint8_t Pending = 0;          /* Indicates if there is a pending task    */>>
int8_t HPrioPendingTask = -1;  /* Indicates the priority of the higest pending
                                   task, or -1 if there is none          *///>>
(...)
/* 
 * HandleTasks (): call from main program
 */
void HandleTasks (void)
{ 
  //while (Pending) {>>
  while (HPrioPendingTask >= 0) { //<<
    //int8_t i=NUMTASKS-1; Pending = 0;>>
    int8_t i=HPrioPendingTask; HPrioPendingTask = -1; //<<
    //while (i>=0 && !Pending) {>>
    while (i>=0 && HPrioPendingTask <= i) { //<<
      Taskp t = &Tasks[i];
      if (t->Activated != t->Invoked) {
        if (t->Flags & TRIGGERED) {
          t->Taskf(); t->Invoked++; 
        }
        else t->Invoked = t->Activated;
      }
      else i--;
} } }

interrupt (TIMERA0_VECTOR) TimerIntrpt (void)
{
  uint8_t i = NUMTASKS-1; 
  do {
    Taskp t = &Tasks[i];
    if (t->Flags & TRIGGERED)
      if (t->Remaining-- == 0) {
        t->Remaining = t->Period-1; 
        t->Activated++;
        //Pending = 1;>>
        if(i > HPrioPendingTask) HPrioPendingTask = i; //<<
      }
  } while (i--);
  //if (Pending) ExitLowPowerMode3();>>
  if (HPrioPendingTask >= 0) ExitLowPowerMode3(); //<<
}
