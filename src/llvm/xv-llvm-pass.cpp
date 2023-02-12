

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/Debug.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;


namespace 
{
    struct XVInstrument : public ModulePass { 
    public: 
        // Pass Identifier variable 
        static char ID; 
        // ModulePass public methods 
        XVInstrument() : ModulePass(ID) {} 
        // StringRef getPassName() override { return "XV fuzz Pass"; } 
        bool runOnModule (Module & M) override; 
    };
}

char XVInstrument::ID = 0;

/* important here for cmd-line use */

static RegisterPass<XVInstrument> P ("xvinst", "XV fuzz instrument"); 
/* Entry point for ModulePass */
bool XVInstrument::runOnModule (Module & M) {
    LLVMContext &CTX = M.getContext();

    IntegerType *Int8Ty  = IntegerType::getInt8Ty(CTX);
    IntegerType *Int32Ty = IntegerType::getInt32Ty(CTX); // 不懂

    printf("[+] xv-llvm-pass");

    GlobalVariable *XvAreaPtr = new GlobalVariable(
        M,
        PointerType::get(Int8Ty, 0),     // point to a int8 (i.e. char *)
        false,
        GlobalVariable::ExternalLinkage,
        0,
        "__xv_area_ptr"
    );

    GlobalVariable *XvPrevLoc = new GlobalVariable(
        M,
        Int32Ty,
        false,
        GlobalVariable::ExternalLinkage,
        0,
        "__xv_prev_loc"
    );

    uint64_t inst_blocks = 0;
    
    for(auto &F : M){
        for(auto &BB : F){

            BasicBlock::iterator IP = BB.getFirstInsertionPt();

            IRBuilder<> IRB(&(*IP));
            
            int cur_loc = 0xdeedbeaf;// TODO: temp
            ConstantInt *CurLoc = ConstantInt::get(Int32Ty, cur_loc);

            /* previous location */
            LoadInst *PrevLoc = IRB.CreateLoad(Int32Ty, XvPrevLoc); // TODO:
            // 我推测 MDNode::get(CTX, None) 会取出当前上下文的注册ID
            PrevLoc->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(CTX, None));// TODO:
            Value *PrevLocCasted = IRB.CreateZExt(PrevLoc, IRB.getInt32Ty());

            /* shm pointer */
            LoadInst *AreaPtr = IRB.CreateLoad(PointerType::get(Int8Ty, 0),XvAreaPtr);
            AreaPtr->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(CTX, None));
            Value *MapPtrIdx = IRB.CreateGEP(
                Int32Ty, 
                AreaPtr, 
                IRB.CreateXor(PrevLocCasted, CurLoc)
            );

            /* Update bitmap */

            LoadInst *Counter = IRB.CreateLoad(Int32Ty, MapPtrIdx);
            Counter->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(CTX, None));
            Value *Incr = IRB.CreateAdd(Counter, ConstantInt::get(Int8Ty, 1));

            IRB.CreateStore(Incr, MapPtrIdx)->setMetadata(
                M.getMDKindID("nosanitize"), 
                MDNode::get(CTX, None)
            );

            /* Set prev_loc to cur_loc >> 1 */

            StoreInst *Store = IRB.CreateStore(
                ConstantInt::get(Int32Ty, cur_loc >> 1), 
                XvPrevLoc
            );
            Store->setMetadata(M.getMDKindID("nosanitize"), MDNode::get(CTX, None));

            inst_blocks++;
        }
    }
    if(not inst_blocks) {
        printf("[!] not instrumented target found");
        exit(1);
    }
    return true;
} 

static void registerXvPass(const PassManagerBuilder &,
                            legacy::PassManagerBase &PM) {

  PM.add(new XVInstrument());

}

static RegisterStandardPasses RegisterAFLPass(
    PassManagerBuilder::EP_ModuleOptimizerEarly, registerXvPass);

static RegisterStandardPasses RegisterAFLPass0(
    PassManagerBuilder::EP_EnabledOnOptLevel0, registerXvPass);
