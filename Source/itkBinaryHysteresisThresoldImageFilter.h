
#ifndef _itkBinaryHysteresisThresoldImageFilter_h
#define _itkBinaryHysteresisThresoldImageFilter_h

#include <ctime>
#include <list>

#include <itkImage.h>
#include <itkNeighborhoodIterator.h>
#include <itkImageRegionIterator.h>
#include <itkConstantBoundaryCondition.h>

#include <itkImageToImageFilter.h>

namespace itk
{

/// \brief Computation of Binary thresholding with hysteresis
///
/// Any pixels aveve highThreshold are set to Inside Value. Pixels
/// below lowerThreshold are set to Outside Value. voxels between
/// low and high tresholds will be set to Inside value if they are connected to
/// any voxel with value Inside.

/// TODO:
/// 1. manual instantation
template<class TInputImage,
    class TOutputPixelType = unsigned char>
class ITK_EXPORT BinaryHysteresisThresoldImageFilter :
  public ImageToImageFilter<
    TInputImage,
    ::itk::Image<
      TOutputPixelType,::itk::GetImageDimension<TInputImage>::ImageDimension> >
{
  public:

    //-----------------------------------------------------
    // Typedefs
    //-----------------------------------------------------
    typedef typename itk::Image< TOutputPixelType, TInputImage::ImageDimension>
      TOutputImage;

    /** Standard class typedefs. */
    typedef BinaryHysteresisThresoldImageFilter Self;
    typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
    typedef SmartPointer<Self> Pointer;
    typedef SmartPointer<const Self> ConstPointer;

    /** Method for creation through the object factory */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(BinaryHysteresisThresoldImageFilter, ImageToImageFilter);

    typedef itk::ConstantBoundaryCondition< TOutputImage > OutputBCType;

    typedef typename TInputImage::ConstPointer InputConstPointerType;
    typedef typename TOutputImage::Pointer     OutputPointerType;

    typedef typename TOutputImage::IndexType  OutputIndexType;
    typedef typename TOutputImage::SizeType   OutputSizeType;
    typedef typename TOutputImage::RegionType OutputRegionType;
    typedef typename TOutputImage::PixelType  OutputPixelType;

    typedef typename TInputImage::IndexType InputIndexType;
    typedef typename TInputImage::PixelType InputPixelType;

    typedef itk::ImageRegionConstIterator< TInputImage > InputConstIteratorType;
    typedef itk::ImageRegionIterator< TOutputImage >     OutputIteratorType;
    typedef itk::ConstNeighborhoodIterator< TInputImage >
      InputConstNeighborhoodIteratorType;
    typedef itk::NeighborhoodIterator< TOutputImage >
      OutputNeighborhoodIteratorType;

    /** The dimension of the input and output images. */
    itkStaticConstMacro(InputImageDimension, unsigned int,
      TInputImage::ImageDimension);
    itkStaticConstMacro(OutputImageDimension, unsigned int,
      TOutputImage::ImageDimension);


    //-----------------------------------------------------
    // Methods
    //-----------------------------------------------------

    /** Set the lower threshold. */
    itkSetMacro(LowerThreshold, InputPixelType);
    /** Get the upper threshold . */
    itkGetConstReferenceMacro(LowerThreshold, InputPixelType);
    /** Set the upper threshold. */
    itkSetMacro(UpperThreshold, InputPixelType);
    /** Get the upper threshold . */
    itkGetConstReferenceMacro(UpperThreshold, InputPixelType);
    /** Set the upper threshold. */
    itkSetMacro(InsideValue, OutputPixelType);
    /** Get the upper threshold . */
    itkGetConstReferenceMacro(InsideValue, OutputPixelType);
    /** Set the upper threshold. */
    itkSetMacro(OutsideValue, OutputPixelType);
    /** Get the upper threshold . */
    itkGetConstReferenceMacro(OutsideValue, OutputPixelType);


#ifdef ITK_USE_CONCEPT_CHECKING
    /** Begin concept checking */
    itkConceptMacro(SameDimensionCheck,
      (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
    /** End concept checking */
#endif

  protected:

    /// \brief Default Constructor
    BinaryHysteresisThresoldImageFilter();

    /// \brief Destructor
    virtual ~BinaryHysteresisThresoldImageFilter();


    /** Compute the medial Surface. */
    void GenerateData();

    void PrintSelf(std::ostream& os, Indent indent) const;

    //-----------------------------------------------------
    // Variables
    //-----------------------------------------------------

    InputPixelType  m_LowerThreshold;             // Lower Threshold
    InputPixelType  m_UpperThreshold;             // Upper Threshold
    OutputPixelType m_InsideValue;             // Inside Value
    OutputPixelType m_OutsideValue;             // Inside Value

    InputConstPointerType m_inputImg;
    OutputPointerType     m_outputImg;
    OutputPointerType     m_queueImg;
    OutputRegionType      m_region;

    std::list<OutputIndexType> m_nodeList;
};

#include "itkBinaryHysteresisThresoldImageFilter.txx"

}//end itk namespace

#endif // _itkBinaryHysteresisThresoldImageFilter_h
