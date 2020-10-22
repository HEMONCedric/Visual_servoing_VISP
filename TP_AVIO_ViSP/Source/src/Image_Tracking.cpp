#include <iostream>
#include <AVIO_ViSP/AVIO_ViSPConfig.h>

#include <visp3/core/vpConfig.h>
#if defined VISP_HAVE_X11
#include <visp3/gui/vpDisplayX.h>
#elif defined VISP_HAVE_OPENCV
#include <visp3/gui/vpDisplayOpenCV.h>
#elif defined VISP_HAVE_GDI
#include <visp3/gui/vpDisplayGDI.h>
#endif

#include <visp3/core/vpTime.h>
#include <visp3/io/vpVideoReader.h>
#include <visp3/core/vpImageFilter.h>

int main()
{

try {
    std::string videoname = "./Sequence_carres/Image_carres_%03d.jpeg";

    // Gray level image
    vpImage<unsigned char> I;

    // Video grabber
    vpVideoReader grabber;
    grabber.setFirstFrameIndex(1);
    grabber.setFileName(videoname);
    grabber.open(I);

    std::cout << "video name: " << videoname << std::endl;
    std::cout << "video dimension: " << I.getWidth() << " " << I.getHeight() << std::endl;

    // Display
#ifdef VISP_HAVE_X11
    vpDisplayX d(I);
#elif defined(VISP_HAVE_OPENCV)
    vpDisplayOpenCV d(I);
#elif defined(VISP_HAVE_GDI)
    vpDisplayGDI d(I);
#endif
    vpDisplay::setTitle(I, "Video reader");

    while(!grabber.end())
    {

        double t0 = vpTime::measureTimeMs();

        // Acquisition of next image
        grabber.acquire(I);

        // Image processing
        vpImage<unsigned char> patch(100,100);
        for(int i=0 ; i<100 ; i++)
        {
            for(int j=0 ; j<100 ; j++)
            {
                patch[i][j] = std::max(0, std::min(255, 2*(int)vpImageFilter::derivativeFilterX(I, 300+i,300+j)));
            }
        }
        I.insert(patch, vpImagePoint(300,300));
        
        // Display
        vpColor color(255, 0, 0);
        vpDisplay::display(I);
        vpDisplay::displayCross(I, 100, 100, 5, color, 1);
        vpDisplay::displayCircle(I, 100, 100, 15, color, false ,1);
        vpDisplay::flush(I);
      
        // A click in the image to exit
        if(vpDisplay::getClick(I, false)) break;
        vpTime::wait(t0, 50);

    }
}
catch (vpException &e)
{
    std::cout << "Catch an exception: " << e << std::endl;
}
}
