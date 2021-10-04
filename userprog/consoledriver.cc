#ifdef CHANGED

#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"

static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }

ConsoleDriver::ConsoleDriver(const char *in, const char *out){
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}

ConsoleDriver::~ConsoleDriver(){
    delete console;
    delete writeDone;
    delete readAvail;
}

void ConsoleDriver::PutChar(int ch){
    char chToPut = (char)(ch);
    console->TX (chToPut);	// echo it!
    writeDone->P ();
}

int ConsoleDriver::GetChar(){
    readAvail->P ();
    char ch = console->RX ();

    return (int)(ch);
}

void ConsoleDriver::PutString(const char s[]){
    
    for(int i =0; i < sizeof(s); i++){
        char chToPut = s[i];
        PutChar((int) chToPut);
    }
    
}
void ConsoleDriver::GetString(char *s, int n){
    // ...
}

int copyStringFromMachine(int from, char *to, unsigned size){
    
    int i = 0;
    for(; i < size; i++){
        int chToCopy;
        char val = machine->ReadMem(from+i,1,&chToCopy); //(address,size=1,);
        char ch = (char)chToCopy; 
        strcat(to, &ch);

        if((char)chToCopy == '\0'){
            break;
        }
    }
    if(to[i] == '\0'){
        return i;
    }else{
        char end ='\0';
        strcat(to,&end);
        return i+1;
    } 



}




#endif // CHANGED
