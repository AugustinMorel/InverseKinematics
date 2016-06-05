#include "CMltools.h"

using namespace std;

int testMlpType(const string modelType);
int testLinearType(const string modelType);

int main()
{
  int fail = 0;
  fail += testMlpType("mlp");
  fail += testLinearType("linear");
  cout << "Number of failures: " << fail << endl;
  return 0;
}
int testMlpType(const string type)
{
  string fileName = "matfiles" + ndlstrutil::dirSep() + "testMltools" + type + ".mat";
  int fail = 0;
  CMatrix X;
  X.readMatlabFile(fileName.c_str(), "X");
  CMatrix y;
  y.readMatlabFile(fileName.c_str(), "y");
  CMatrix W1;
  W1.readMatlabFile(fileName.c_str(), "W1");
  int numHidden = W1.getCols();
  CMatrix b1;
  b1.readMatlabFile(fileName.c_str(), "b1");
  CMatrix W2;
  W2.readMatlabFile(fileName.c_str(), "W2");
  CMatrix b2;
  b2.readMatlabFile(fileName.c_str(), "b2");
  
  
  CMlpMapping modelInit(&X, &y, numHidden, 2);
  modelInit.setWeights(W1, 0);
  modelInit.setWeights(W2, 1);
  modelInit.setBias(b1, 0);
  modelInit.setBias(b2, 1);


  CMlpMapping model(&X, &y, fileName.c_str(), "model", 0);
  if(model.equals(modelInit))
  { 
    cout << model.getName() << " initial model passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << "." << endl;
    cout << "Matlab loaded model " << endl;
    model.display(cout);
    cout << "C++ Initialised Model " << endl;
    modelInit.display(cout);
    fail++;
  }

  CMatrix fileParams;
  fileParams.readMatlabFile(fileName.c_str(), "params");
  CMatrix params(1, model.getOptNumParams());
  model.getOptParams(params);
  if(params.equals(fileParams)) 
  {
    cout << "Parameter match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " parameter match." << endl;
    cout << "Matlab loaded params: " << fileParams << endl;
    cout << "C++ Initialised params: " << params << endl;
    fail++;
  }

  CMatrix fileOut;
  fileOut.readMatlabFile(fileName.c_str(), "out");
  CMatrix out(X.getRows(), y.getCols());
  model.out(out, X);

  if(out.equals(fileOut)) 
  {
    cout << model.getName() << " output match passed." << endl;
  }
  else 
  {
    fileOut.trans();
    out.trans();
    cout << "FAILURE: " << model.getName() << " output match." << endl;
    cout << "Matlab output " << fileOut << endl;
    cout << "C++ output " << out << endl;
    fail++;
  }

  CMatrix fileOutGrad;
  fileOutGrad.readMatlabFile(fileName.c_str(), "outGrad");
  CMatrix outGrad(1, model.getOptNumParams());
  model.outGradParams(outGrad, X, 0, 0);

  if(outGrad.equals(fileOutGrad)) 
  {
    cout << model.getName() << " output gradient match passed." << endl;
  }
  else 
  {
    fileOut.trans();
    out.trans();
    cout << "FAILURE: " << model.getName() << " output gradient match." << endl;
    cout << "Matlab output gradient " << fileOutGrad << endl;
    cout << "C++ output gradient " << outGrad << endl;
    cout << "Max abs difference" << fileOutGrad.maxAbsDiff(outGrad) << endl;
    fail++;
  }

  CMatrix fileGrads;
  fileGrads.readMatlabFile(fileName.c_str(), "grads");
  CMatrix grads(1, model.getOptNumParams());
  model.logLikelihoodGradient(grads);
  if(grads.equals(fileGrads)) 
  {
    cout << model.getName() << " Gradient match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " gradient match." << endl;
    cout << "Matlab Gradient " << fileGrads << endl;
    cout << "C++ Gradient " << grads << endl;
    fail++;
  }

  CMatrix fileLl;
  fileLl.readMatlabFile(fileName.c_str(), "ll");
  CMatrix ll(1, 1);
  ll.setVal(model.logLikelihood(), 0, 0);
  if(ll.equals(fileLl)) 
  {
    cout << model.getName() << " Log Likelihood match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " log likelihood match." << endl;
    cout << "Matlab Log Likelihood " << fileLl << endl;
    cout << "C++ Log Likelihood " << ll << endl;
    fail++;
  }
  model.checkGradients();
  model.toFile("mlp_model", "Generated by testMltools");
  CMlpMapping* fileModel = new CMlpMapping();
  fileModel->fromFile("mlp_model");
  if(model.equals(*fileModel))
  {
    cout << model.getName() << " write to file and read back match check passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " write to file and read back match check failed." << endl;
    cout << "Written model " << endl;
    model.display(cout);
    cout << "Read in model " << endl;
    fileModel->display(cout);
    fail++;
  }
  return fail;
}
int testLinearType(const string type)
{
  string fileName = "matfiles" + ndlstrutil::dirSep() + "testMltools" + type + ".mat";
  int fail = 0;
  CMatrix X;
  X.readMatlabFile(fileName.c_str(), "X");
  CMatrix y;
  y.readMatlabFile(fileName.c_str(), "y");
  CMatrix W;
  W.readMatlabFile(fileName.c_str(), "W");
  CMatrix b;
  b.readMatlabFile(fileName.c_str(), "b");
  
  
  CLinearMapping modelInit(&X, &y, 2);
  modelInit.setWeights(W, 0);
  modelInit.setBias(b, 0);


  CLinearMapping model(&X, &y, fileName.c_str(), "model", 0);

  if(model.equals(modelInit))
  { 
    cout << model.getName() << " initial model passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << "." << endl;
    cout << "Matlab loaded model " << endl;
    model.display(cout);
    cout << "C++ Initialised Model " << endl;
    modelInit.display(cout);
    fail++;
  }

  CMatrix fileParams;
  fileParams.readMatlabFile(fileName.c_str(), "params");
  CMatrix params(1, model.getOptNumParams());
  model.getOptParams(params);
  if(params.equals(fileParams)) 
  {
    cout << "Parameter match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " parameter match." << endl;
    cout << "Matlab loaded params: " << fileParams << endl;
    cout << "C++ Initialised params: " << params << endl;
    fail++;
  }

  CMatrix fileOut;
  fileOut.readMatlabFile(fileName.c_str(), "out");
  CMatrix out(X.getRows(), y.getCols());
  model.out(out, X);

  if(out.equals(fileOut)) 
  {
    cout << model.getName() << " output match passed." << endl;
  }
  else 
  {
    fileOut.trans();
    out.trans();
    cout << "FAILURE: " << model.getName() << " output match." << endl;
    cout << "Matlab output " << fileOut << endl;
    cout << "C++ output " << out << endl;
    fail++;
  }

  CMatrix fileOutGrad;
  fileOutGrad.readMatlabFile(fileName.c_str(), "outGrad");
  CMatrix outGrad(1, model.getOptNumParams());
  model.outGradParams(outGrad, X, 0, 0);

  if(outGrad.equals(fileOutGrad)) 
  {
    cout << model.getName() << " output gradient match passed." << endl;
  }
  else 
  {
    fileOut.trans();
    out.trans();
    cout << "FAILURE: " << model.getName() << " output gradient match." << endl;
    cout << "Matlab output gradient " << fileOutGrad << endl;
    cout << "C++ output gradient " << outGrad << endl;
    cout << "Max abs difference" << fileOutGrad.maxAbsDiff(outGrad) << endl;
    fail++;
  }

  CMatrix fileGrads;
  fileGrads.readMatlabFile(fileName.c_str(), "grads");
  CMatrix grads(1, model.getOptNumParams());
  model.logLikelihoodGradient(grads);
  if(grads.equals(fileGrads)) 
  {
    cout << model.getName() << " Gradient match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " gradient match." << endl;
    cout << "Matlab Gradient " << fileGrads << endl;
    cout << "C++ Gradient " << grads << endl;
    fail++;
  }

  CMatrix fileLl;
  fileLl.readMatlabFile(fileName.c_str(), "ll");
  CMatrix ll(1, 1);
  ll.setVal(model.logLikelihood(), 0, 0);
  if(ll.equals(fileLl)) 
  {
    cout << model.getName() << " Log Likelihood match passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " log likelihood match." << endl;
    cout << "Matlab Log Likelihood " << fileLl << endl;
    cout << "C++ Log Likelihood " << ll << endl;
    fail++;
  }
  model.checkGradients();
  model.toFile("linear_model", "Generated by testMltools");
  CLinearMapping* fileModel = new CLinearMapping();
  fileModel->fromFile("linear_model");
  if(model.equals(*fileModel))
  {
    cout << model.getName() << " write to file and read back match check passed." << endl;
  }
  else 
  {
    cout << "FAILURE: " << model.getName() << " write to file and read back match check failed." << endl;
    cout << "Written model " << endl;
    model.display(cout);
    cout << "Read in model " << endl;
    fileModel->display(cout);
    fail++;
  }
  return fail;
}
