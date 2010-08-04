// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#ifndef _IMAGEPROCESSMODULE_H_
#define _IMAGEPROCESSMODULE_H_

//YARP include
#include <yarp/os/all.h>
#include <yarp/sig/all.h>

//openCV include
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

//=============================================================================
// GTK Includes 
//=============================================================================
#include <gtk/gtk.h>
//=============================================================================
// YARPImgRecv Includes 
//=============================================================================
#include <iCub/YarpImage2Pixbuf.h>
#include <iCub/YARPImgRecv.h>

//within Project Include
#include <iCub/WatershedOperator.h>
#include <iCub/SalienceOperator.h>
#include <string>


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;



/**
*
@ingroup icub_module
\defgroup icub_saliencyBlobFinderInterface saliencyBlobFinderInterface

Module graphical interface for saliencyBlobFinder

\section intro_sec Description
This module is purely an interface towards the module saliencyBlobFinder. It sends commands via /command:o port as bottles.
The protocol respects the directives that the saliencyBlobFinder requires.
In addition this interface is able to draw an image. This useful functionality allows the user to visualise straigh away important information 
and result of his/her interaction


The module does:
-   stream the command to the saliencyBlobFinder module
-   visualise an image in the graphics interface

\section How to use it
At the moment it is possible to tune some parameter used by the saliencyBlobFinder. 

In particular concerning scales:
- kcoeff.TD: weight for the top-down algorithm for the computation of saliency. The greater the value the more salient 
the blobs which have similar target-colour 
-red intensity: set the level in the red plane of the targetColour
-green intensity: set the level in the green plane of the targetColour
-blue intensity: set the level in the blue plane of the targetColour
- kcoeff.BU: weight for the bottom-up algorithm for the computation of saliency. The greater the value the more salient 
the blobs which are isolated (it should be set complementary to kCoeff TD)
-maxBlob dimension: set the maximum dimension of the blobs
-minBlob dimension: set the minimum dimension of the blobs


In particular concerning checkboxes, these set the output of the saliencyBlobFinder:
- watershed: result of the watershed operation
- tagged: grayscale image composed by the pixel-wise tags of the blobs
- contrastLP: saliency map of all the blobs 
- foveaBlob: blob of the fovea
- maxSaliencyBlob: most salient blob in the saliency map



\image html saliencyBlobFinder.png

\section lib_sec Libraries
YARP
GTK

\section parameters_sec Parameters
--name : name of the module and relative port name

 
\section portsa_sec Ports Accessed
/blobFinder/cmd


\section portsc_sec Ports Created
Input ports:
none
Outports
- /watershed/command:o

\section in_files_sec Input Data Files
none

\section out_data_sec Output Data Files
none
 
\section conf_file_sec Configuration Files
none

\section tested_os_sec Tested OS
Linux and Windows.

\section example_sec Example Instantiation of the Module
saliencyBlobFinderInterface 


\author Francesco Rea

Copyright (C) 2008 RobotCub Consortium

CopyPolicy: Released under the terms of the GNU GPL v2.0.

**/


class WatershedModule : public Thread {
private:
    /**
    * a port for reading the edge image 
    */
    BufferedPort<ImageOf<PixelRgb> > port_in; // 
    /**
    * a port for reading the input Image of the Red Plane
    */
    BufferedPort<ImageOf<PixelRgb> > portRedPlane; // 
    /**
    * a port for reading the input Image of the Green Plane
    */
    BufferedPort<ImageOf<PixelRgb> > portGreenPlane; //
    /**
    * a port for reading the input Image of the Blue Plane 
    */
    BufferedPort<ImageOf<PixelRgb> > portBluePlane; // 
    /**
    * a port for reading the R+G- colour opponency Image
    */
    BufferedPort<ImageOf<PixelRgb> > portRG; // 
    /**
    * a port for reading the G+R- colour opponency Image
    */
    BufferedPort<ImageOf<PixelRgb> > portGR; // 
    /**
    * a port for reading the B+Y- colour opponency Image 
    */
    BufferedPort<ImageOf<PixelRgb> > portBY; // 
    /**
    * port where the processed image is buffered out
    */
    BufferedPort<ImageOf<PixelRgb> > port_out; //
    /**
    * port where the image of the found blob is put
    */
    BufferedPort<ImageOf<PixelBgr> > port_Blobs; //
    /**
    * root name of all the port connections
    */
     ConstString name;
        
public:
    //------------------ PUBLIC METHODS -----------------------------------

    /**
    * default constructor
    */
    WatershedModule();
    /**
    * default destructor
    */
    ~WatershedModule();
    /**
    * function called after the stop is called
    */
    void onStop();

    /**
    *	initialization of the thread 
    */
    bool threadInit();
    /**
    * active loop of the thread
    */
    void run();
    /**
    *	releases the thread
    */
    void threadRelease();
    /**
    * interrupts any communications or resource usage
    */
    bool interrupt(); // 
    /**
    * closes all this thread safely
    */
    bool close(); //
    /**
    * function that resizes all the images and all the objects that are dependent from the size of the inputimage
    */
    void resizeImages(int width, int height);

    /**
    * set name of the module for any port connect function
    * @param name starting root of all the ports
    */
    void setName(ConstString name);
    /**
    * returns the name of the module
    */
    ConstString getName();
    /**
    * appends the suffix to the name of the module and returns the result
    */
    ConstString getName(ConstString suffix);
    /**
    * create the main Window
    */
    GtkWidget* createMainWindow(); //
    /**
    * creates the menu bar
    */
    GtkWidget* createMenubar(void); //
    /**
    * set the images to the output ports
    */
    bool outPorts(); //
    /**
    * set different components of the module up
    */
    void setUp();
    /**
    * set the reference to the module that has created this thread
    */
    void setModule(void* refModule);
    /**
    * creats some components of the module
    */
    void createObjects();
    /**
    * open some additional ports
    */
    bool openPorts();
    /**
    * close additional ports
    */
    bool closePorts();
    /**
    * applies the rain falling watershed
    */
    void rain();
    /**
    * draws all the blobs in the module
    */
    void drawAllBlobs(bool stable);
    
    //----------------- PUBLIC ATTRIBUTES -----------------------------
    /**
    * counter
    */
    int ct;

    /**
    * width of images which the module is dealing with
    */
    int width;
    /**
    * height of images which the module is dealing with
    */
    int height;
    /**
    * saliencyTOT linear combination Ktd coefficient (TOP DOWN saliency weight)
    */
    double salienceTD;
    /**
    * saliencyTOT linear combination Kbu coefficient (BOTTOM-UP saliency weight)
    */
    double salienceBU;
    /**
    * time constant of the robot in seconds for the format (x,y,z)
    */
    double reactivity;
    /**
    * time constant of the robot in seconds for the format (set img x y)
    */
    double timeCentroid;
    /**
    * red intensity of the target that has been found 
    */
    double targetRED;
    /**
    * green intensity of the target that has been found 
    */
    double targetGREEN;
    /**
    * blue intensity of the target that has been found 
    */
    double targetBLUE;
    /**
    * R+G- value for the search
    */
    int searchRG;
    /**
    * G+R- value for the search
    */
    int searchGR;
    /**
    * B+Y- value for the search
    */
    int searchBY;

    /**
    * Output Bottle Port for any possible command
    */
    yarp::os::BufferedPort<yarp::os::Bottle> *_pOutPort;
    /**
    * Output port that send out a PixelRgb image
    */
    yarp::os::BufferedPort<ImageOf<PixelRgb> > *_pOutPort2;
    /**
    * Output port that send out a PixelRgb image
    */
    yarp::os::BufferedPort<ImageOf<PixelRgb> > *_pOutPort3;
    /**
    * port for eventually streaming the command
    */
    yarp::os::BufferedPort<Bottle > *commandPort;
    /**
    *Output Bottle Container
    */
    Bottle _outBottle;
    /**
    * reference to the watershed operator
    */
    WatershedOperator *wOperator;
    /**
    * reference to the salience operator
    */
    SalienceOperator *salience;
    /**
    * number of blobs
    */
    int max_tag;
    /**
    * vector of boolean which tells whether there is a blob or not
    */
    char* blobList; //
    /**
    *vector of tags to the sequence of blobs
    */
    ImageOf<PixelInt>* tagged;  //
    /**
    * image result of the function outContrastLP
    */
    ImageOf<PixelMono> *outContrastLP;
    /**
    * image result of the function meanColourLP;
    */
    ImageOf<PixelBgr> *outMeanColourLP;
    /**
    * image of the most salient blob
    */
    ImageOf<PixelMono>* maxSalienceBlob_img;
    /**
    * pointer to the input image of the red plane
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputRed; //
    /**
    * pointer to the input image of the green plane
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputGreen; //
    /**
    * pointer to the input image of the blue plane
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputBlue; //
    /**
    * pointer to the input image of the R+G- colour opponency
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputRG;  //
    /**
    * pointer to the input image of the G+R- colour opponency
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputGR;  //
    /**
    * pointer to the input image of the B+Y- colour opponency
    */
    yarp::sig::ImageOf<yarp::sig::PixelMono> *ptr_inputBY; //
    /**
    * input image of the opponency R+G-
    */
    ImageOf<PixelMono> *_inputImgRGS;
    /**
    * input image of the opponency G+R-
    */
    ImageOf<PixelMono> *_inputImgGRS;
    /**
    * input image of the opponency B+Y-
    */
    ImageOf<PixelMono> *_inputImgBYS;
    /**
    * image of the fovea blob
    */
    ImageOf<PixelMono> *blobFov;
    /**
    * image which is plotted in the drawing area
    */
    ImageOf<PixelRgb> *image_out; //
    //---------- flags --------------------------
    /**
    * flag for drawing contrastLP
    */
    bool contrastLP_flag;
    /**
    * flag for drawing meanColourImage
    */
    bool meanColour_flag;
    /**
    * flag for drawing blobCatalog
    */
    bool blobCataloged_flag;
    /**
    * flag for drawing foveaBlob
    */
    bool foveaBlob_flag;
    /**
    * flag for drawing colorVQ
    */
    bool colorVQ_flag;
    /**
    * flag for drawing maxSaliencyBlob
    */
    bool maxSaliencyBlob_flag;
    /**
    * flag for drawing blobList
    */
    bool blobList_flag;
    /**
    * flag for the drawings
    */
    bool tagged_flag;
    /**
    * flag for drawing watershed image
    */
    bool watershed_flag;
    /**
    * flag for drawing redPlane image
    */
    bool redPlane_flag;
    /**
    * flag for drawing greenPlane image
    */
    bool greenPlane_flag;
    /**
    * flag for drawing bluePlane image
    */
    bool bluePlane_flag;
    /**
    * flag for drawing RedGreen Opponency Map
    */
    bool RG_flag;
    /**
    * flag for drawing GreenRed Opponency Map
    */
    bool GR_flag;
    /**
    * flag for drawing BlueYellow Opponency Map
    */
    bool BY_flag;
    /**
    * flag that indicates if the inputImage has been acquired
    */
    bool inputImage_flag;
    /**
    * flag that indicates if the images have been resized
    */
    bool resized_flag;
    /**
    * flag that indicates that Opponency images are not ready
    */
    bool noOpponencies_flag;
    /**
    * flag that indicates that Plane images are not ready
    */
    bool noPlanes_flag;
    /**
    * maxBLOB dimension
    */
    int maxBLOB;
    /**
    * minBLOB dimension
    */
    int minBLOB;

    //----checkButtons
    GtkWidget *buttonCheckGreen,*buttonCheckRed,*buttonCheckBlue;
    /**
    * max saliency blob
    */
    YARPBox* max_boxes;

    /**
    * string for the the command
    */
    std::string* message;
    
    
};

#endif //_WATERSHEDMODULE_H_

//----- end-of-file --- ( next line intentionally left blank ) ------------------
