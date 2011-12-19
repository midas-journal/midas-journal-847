#include "itkBinaryHysteresisThresoldImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

int main(int argc, char* argv []) 
{
  if (argc != 5)
    {
    std::cerr << "Incorrect parameters: \nUsage:" << 
    argv[0] << " Input-image th1 th2 Output-image\n"<< std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<unsigned char,2>   uc2dImg;
  typedef itk::ImageFileReader<uc2dImg> ReaderFilter;
  ReaderFilter::Pointer reader = ReaderFilter::New();
  reader->SetFileName(argv[1]);
  typedef itk::BinaryHysteresisThresoldImageFilter<uc2dImg> Hysteresis;
  Hysteresis::Pointer thr = Hysteresis::New();
  thr->SetInput(reader->GetOutput());
  thr->SetLowerThreshold(atoi(argv[2]));
  thr->SetUpperThreshold(atoi(argv[3]));
  typedef itk::ImageFileWriter<uc2dImg> WriterFilter;
  WriterFilter::Pointer writer = WriterFilter::New();
  writer->SetFileName(argv[4]);
  writer->SetInput(thr->GetOutput());
    
  try 
    {
    writer->Update();
    }
  catch(itk::ExceptionObject& e) 
    {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
    }
  catch(...) 
    {
    std::cerr << "Unexpected error\n";
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
