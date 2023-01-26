#include "worker.h"

int status = 0; //0: idle, 1: running, 2: failure

char* inst = 0;
int instCount = 0;
int instPtr = 0;
int runOpCount = 0;

char* wOutput = 0;
int wOutputPtr = 0;

char* wInput = 0;
int wInputPtr = 0;

uint8_t* bfStack = 0;
int stackPtr = 0;
int stackSize = BF_STACK_INITIAL_SIZE;
int stackSizeReal = 0;

bool validateInstPtr(){
    if(instPtr > instCount || instPtr < 0){
        return false;
    }
    return true;
}

bool validateStackPtr(){
    if(stackPtr > stackSize || stackPtr < 0){
        return false;
    }
    return true;
}

char* workerGetOutput(){
    return wOutput;
}

int getStackSize(){
    return stackSizeReal;
}

int getOpCount(){
    return runOpCount;
}

int getStatus(){
    return status;
}

void initWorker(BFApp* app){
    //rebuild output
    if(wOutput){ free(wOutput); }
    wOutput = (char*)malloc(BF_OUTPUT_SIZE);
    wOutputPtr = 0;

    //rebuild stack
    if(bfStack){ free(bfStack); }
    bfStack = (uint8_t*)malloc(BF_STACK_INITIAL_SIZE);
    memset(bfStack, 0x00, BF_STACK_INITIAL_SIZE);
    stackSize = BF_STACK_INITIAL_SIZE;
    stackSizeReal = 0;
    stackPtr = 0;

    //set instructions
    inst = app->dataBuffer;
    instCount = app->dataSize;
    instPtr = 0;
    runOpCount = 0;

    //set input
    wInput = app->inputBuffer;
    wInputPtr = 0;

    //set status
    status = 0;
}

void rShift(){
    runOpCount++;
    stackPtr++;
    if(!validateStackPtr()){ status = 2; return; }

    while(stackPtr > stackSize){
        stackSize += BF_STACK_STEP_SIZE;
        void* tmp = realloc(bfStack, stackSize);

        if(!tmp){ 
            status = 2;
            return;
        }

        memset((tmp + stackSize) - BF_STACK_STEP_SIZE, 0x00, BF_STACK_STEP_SIZE);
        bfStack = (uint8_t*)tmp;
    };
    if(stackPtr > stackSizeReal){ 
        stackSizeReal = stackPtr; 
    }
}

void lShift(){
    runOpCount++;
    stackPtr--;
    if(!validateStackPtr()){ status = 2; return; }
}

void inc(){
    runOpCount++;
    if(!validateStackPtr()){ status = 2; return; }
    bfStack[stackPtr]++;
}

void dec(){
    runOpCount++;
    if(!validateStackPtr()){ status = 2; return; }
    bfStack[stackPtr]--;
}

void print(){
    runOpCount++;
    wOutput[wOutputPtr] = bfStack[stackPtr];
    wOutputPtr++;
    if(wOutputPtr > (BF_OUTPUT_SIZE - 1)){ wOutputPtr = 0;}
}

void input(){
    runOpCount++;
    
    bfStack[stackPtr] = (uint8_t)wInput[wInputPtr];
    if(wInput[wInputPtr] == 0x00 || wInputPtr >= 64){
        wInputPtr = 0;
    }
    else{
        wInputPtr++;
    }
}

void loop() {
    runOpCount++;
	if (bfStack[stackPtr] == 0) {
		int loopCount = 1;
		while (loopCount > 0) {
			instPtr++;
            if(!validateInstPtr()){ status = 2; return; }
			if (inst[instPtr] == '[') { loopCount++; }
			else if (inst[instPtr] == ']') { loopCount--; }
		}
	}
}

void endLoop() {
    runOpCount++;
	if (bfStack[stackPtr] != 0) {
		int loopCount = 1;
		while (loopCount > 0) {
			instPtr--;
            if(!validateInstPtr()){ status = 2; return; }
			if (inst[instPtr] == ']') { loopCount++; }
			else if (inst[instPtr] == '[') { loopCount--; }
		}
	}
}

void beginWorker(){
    status = 1;
    while (inst[instPtr] != 0x00) {
        if(status == 2){ return; }
        switch (inst[instPtr]) {
            case '>':
                rShift();
                break;
            case '<':
                lShift();
                break;

            case '+':
                inc();
                break;
                
            case '-':
                dec();
                break;
                
            case '.':
                print();
                break;
            
            case ',':
                input();
                break;
                
            case '[':
                loop();
                break;
                
            case ']':
                endLoop();
                break;
                
            default:
                break;
        }
        instPtr++;
        if(!validateInstPtr()){ status = 2; return; }
    }
    status = 0;
}