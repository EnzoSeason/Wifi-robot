// Includes, project
#include "sobel-component.hpp"

// Includes, standar
#include <cmath>
#include <math.h>

using namespace sy31;

////////////////////////////////////////////////////////////////
// RTMaps - Input
////////////////////////////////////////////////////////////////
MAPS_BEGIN_INPUTS_DEFINITION(SobelComponent)
  MAPS_INPUT("Source", MAPS::FilterIplImage, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Output
////////////////////////////////////////////////////////////////
MAPS_BEGIN_OUTPUTS_DEFINITION(SobelComponent)
  MAPS_OUTPUT("Gray", MAPS::IplImage, NULL, NULL, 1)
  MAPS_OUTPUT("Sobel", MAPS::IplImage, NULL, NULL, 1)
MAPS_END_OUTPUTS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Properties
////////////////////////////////////////////////////////////////
MAPS_BEGIN_PROPERTIES_DEFINITION(SobelComponent)
MAPS_END_PROPERTIES_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Actions
////////////////////////////////////////////////////////////////
MAPS_BEGIN_ACTIONS_DEFINITION(SobelComponent)
MAPS_END_ACTIONS_DEFINITION

////////////////////////////////////////////////////////////////
// RTMaps - Definition
////////////////////////////////////////////////////////////////
MAPS_COMPONENT_DEFINITION(
	SobelComponent, "SobelComponent", "1.0", 128,
	MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
  1, // Nb of inputs
	2, // Nb of outputs
	0, // Nb of properties
	0) // Nb of actions

void
SobelComponent::Birth()
{
  mIsAllocated = false;
}

void
SobelComponent::Core() 
{
  MAPSIOElt* input = StartReading(Input("Source"));
  IplImage& source = input->IplImage();
  if (!mIsAllocated)
    allocateOutputs(source);

  MAPSIOElt* outSobel = StartWriting(Output("Sobel"));
  MAPSIOElt* outGray  = StartWriting(Output("Gray"));

  IplImage& sobel = outSobel->IplImage();
  IplImage& gray  = outGray->IplImage();

  /* Convertir l'image en niveau de gris */
  rgb2gray(source, gray);

  /* Calculer le filtre de Sobel */
  compute_sobel(gray, sobel);

  /* Set the timestamps */
  outSobel->Timestamp() = MAPS::CurrentTime();
  outGray->Timestamp()  = MAPS::CurrentTime();

  StopReading(Input("Source"));
  StopWriting(outSobel);
  StopWriting(outGray);
}

void
SobelComponent::Death()
{
  // Couic! :(
}

void
SobelComponent::allocateOutputs(IplImage const& src)
{
  IplImage model = MAPS::IplImageModel(src.width, src.height, MAPS_CHANNELSEQ_GRAY);

  Output("Sobel").AllocOutputBufferIplImage(model);
  Output("Gray").AllocOutputBufferIplImage(model);

  mIsAllocated = true;
}

void
SobelComponent::rgb2gray(IplImage const& src, IplImage& dst)
{
	for(int i = 0; i<(src.width * src.height); i ++){
		dst.imageData[i] = src.imageData[3*i] * 0.299 +  src.imageData[3*i + 1] * 0.587 + src.imageData[3*i +2] * 0.114;
	}
}

int
SobelComponent::convolution(IplImage const& src, int x, int y, int const* mask)
{
 /* TODO */
	int tab[9];
	int index;
	int sum = 0;
	for(int i = 0;i < 9;i++){
		tab[i] = 0;
	}
	index = x * src.width + y;
	if(x > 0 && y > 0 && x < src.height && y < src.width){  
        tab[0] = src.imageData[index - src.width -1];
		tab[1] = src.imageData[index - src.width];
		tab[2] = src.imageData[index - src.width +1];
		tab[3] = src.imageData[index-1];
		tab[4] = src.imageData[index];
		tab[5] = src.imageData[index+1];
		tab[6] = src.imageData[index + src.width -1];
		tab[7] = src.imageData[index + src.width];
		tab[8] = src.imageData[index + src.width+1];
		for(int i = 0; i<9; i++){
		    sum = sum + tab[i]*mask[i];
	    }
	    return sum;
    }
	return 0;
}

void
SobelComponent::compute_sobel(IplImage const& src, IplImage& dst)
{
  /* Exemple: dst.imageData[y * src.width + x] = value; */
	int maskx[9]={-1,0,1,-2,0,2,-1,0,1};
	int masky[9]={1,2,1,0,0,0,-1,-2,-1};
	int index = 0;
	for(int i = 0; i<src.height; i++){
		for(int j = 0; j<src.width; j++){
			dst.imageData[index] =(int) sqrt((double)convolution(src,i,j,maskx)*convolution(src,i,j,maskx)+convolution(src,i,j,masky)*convolution(src,i,j,masky));
			index ++;
		}
	}
}

