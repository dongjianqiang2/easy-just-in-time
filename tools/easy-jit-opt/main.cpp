#include "StaticPasses.h"

#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/InitLLVM.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/ToolOutputFile.h>

int main(int argc, char **argv) {
  llvm::InitLLVM X(argc, argv);

  llvm::cl::OptionCategory EasyJitCategory("easy-jit options");
  llvm::cl::opt<std::string> Input(llvm::cl::Positional,
                                   llvm::cl::desc("<input llvm ir/bitcode>"),
                                   llvm::cl::Required,
                                   llvm::cl::cat(EasyJitCategory));
  llvm::cl::opt<std::string> Output("o", llvm::cl::desc("Output bitcode file"),
                                    llvm::cl::value_desc("filename"),
                                    llvm::cl::init("-"),
                                    llvm::cl::cat(EasyJitCategory));

  llvm::cl::ParseCommandLineOptions(argc, argv, "easy-jit optimization pass\n");

  llvm::LLVMContext Context;
  llvm::SMDiagnostic Diag;
  std::unique_ptr<llvm::Module> M = llvm::parseIRFile(Input, Diag, Context);
  if (!M) {
    Diag.print(argv[0], llvm::errs());
    return 1;
  }

  llvm::legacy::PassManager PM;
  PM.add(easy::createRegisterBitcodePass());
  PM.run(*M);

  std::error_code EC;
  llvm::sys::fs::OpenFlags Flags = llvm::sys::fs::OF_None;
  auto Out = std::make_unique<llvm::ToolOutputFile>(Output, EC, Flags);
  if (EC) {
    llvm::errs() << "failed to open output file '" << Output << "': " << EC.message() << "\n";
    return 1;
  }

  llvm::WriteBitcodeToFile(*M, Out->os());
  Out->keep();
  return 0;
}
