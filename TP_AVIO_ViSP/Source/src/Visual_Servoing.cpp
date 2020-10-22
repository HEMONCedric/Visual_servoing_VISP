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
#include <visp3/gui/vpProjectionDisplay.h>
#include <visp3/robot/vpSimulatorCamera.h>
#include <visp3/robot/vpWireFrameSimulator.h>
#include <visp3/visual_features/vpFeatureBuilder.h>
#include <visp3/gui/vpPlot.h>
#include <visp3/vs/vpServo.h>
#include<windows.h>
#include <visp3/vs/vpServoDisplay.h>


void display_trajectory(const vpImage<unsigned char> &I, std::vector<vpColVector> &point, const vpCameraParameters &cam);
vpColVector changeFrame(const vpColVector &point, const vpHomogeneousMatrix &M);
vpColVector projectPointOnImage(const vpColVector &point, const vpCameraParameters &cam);
vpMatrix computeInteractionMatrixOfPoint(double u, double v, const vpCameraParameters &cam, double Z);


int main()
{
try{
    // Time
    double time = 0;
    
    // Pose of the object in the camera frame at the desired position
    vpHomogeneousMatrix cdMo(0, 0, 0.75, 0, 0, 0);
    
    // Initial pose of the object in the camera frame
    vpHomogeneousMatrix cMo(0.15, -0.1, 1., vpMath::rad(10), vpMath::rad(-10), vpMath::rad(50));

    // Position of the four points on the object in the object frame
    std::vector<vpColVector> pointO(4, vpColVector(3,0));
    pointO.at(0)[0] = -0.1; pointO.at(0)[1] = -0.1; pointO.at(0)[2] = 0;
    pointO.at(1)[0] = +0.1; pointO.at(1)[1] = -0.1; pointO.at(1)[2] = 0;
    pointO.at(2)[0] = +0.1; pointO.at(2)[1] = +0.1; pointO.at(2)[2] = 0;
    pointO.at(3)[0] = -0.1; pointO.at(3)[1] = +0.1; pointO.at(3)[2] = 0;
    
    // Position of the four points on the object in the camera frame
    std::vector<vpColVector> pointC(4, vpColVector(3,0));
    for(int i=0 ; i<4 ; i++) pointC.at(i) = changeFrame(pointO.at(i), cMo);
    
    // Camera simulator
    vpSimulatorCamera robot;
    robot.setSamplingTime(0.040);
    
    // Current pose of the camera
    vpHomogeneousMatrix wMc;
    robot.getPosition(wMc);
    
    // Pose of the object in the world frame
    vpHomogeneousMatrix wMo = wMc * cMo;
    
    // Display the camera view and external view
    vpImage<unsigned char> Iint(480, 640, 0);
    vpImage<unsigned char> Iext(480, 640, 0);
#if defined VISP_HAVE_X11
    vpDisplayX displayInt(Iint, 0, 0, "Internal view");
    vpDisplayX displayExt(Iext, 670, 0, "External view");
#elif defined VISP_HAVE_OPENCV
    vpDisplayOpenCV displayInt(Iint, 0, 0, "Internal view");
    vpDisplayOpenCV displayExt(Iext, 670, 0, "External view");
#elif defined VISP_HAVE_GDI
    vpDisplayGDI displayInt(Iint, 0, 0, "Internal view");
    vpDisplayGDI displayExt(Iext, 670, 0, "External view");
#else
    std::cout << "No image viewer is available..." << std::endl;
#endif
    
    // Display velocities
    vpPlot velPlot(2, 700, 700, -1, -1, "Camera velocity");
    velPlot.initGraph(0,3);
    velPlot.setTitle(0, "Translation");
    velPlot.setLegend(0,0, "Vx");
    velPlot.setLegend(0,1, "Vy");
    velPlot.setLegend(0,2, "Vz");
    velPlot.initGraph(1,3);
    velPlot.setTitle(1, "Rotation");
    velPlot.setLegend(1,0, "Wx");
    velPlot.setLegend(1,1, "Wy");
    velPlot.setLegend(1,2, "Wz");
    
    // Display errors in image
    vpPlot errorPlot(4, 700, 700, -1, -1, "Image errors");
    for (int i=0 ; i<4 ; i++)
    {
        errorPlot.initGraph(i,2);
        char text[1024];
        sprintf(text, "Error point %d", i);
        errorPlot.setTitle(0, text);
        errorPlot.setLegend(i,0, "Ex");
        errorPlot.setLegend(i,1, "Ey");
    }
    
    // Display camera position error
    vpPlot positionErrorPlot(1, 700, 700, -1, -1, "Camera position error");
    positionErrorPlot.initGraph(0,3);
    positionErrorPlot.setLegend(0,0, "Ex");
    positionErrorPlot.setLegend(0,1, "Ey");
    positionErrorPlot.setLegend(0,2, "Ez");

    // Camera parameters
    double f = 840;
    double u0 = 5*(Iint.getWidth() / 2);
    double v0 = 5*(Iint.getHeight() / 2);
    vpCameraParameters cam(f, f, u0, v0);
    vpHomogeneousMatrix cextMo(0, 0, 3, 0, 0, 0);

    // Object simulator
    vpWireFrameSimulator sim;
    sim.initScene(vpWireFrameSimulator::PLATE, vpWireFrameSimulator::D_STANDARD);
    sim.setCameraPositionRelObj(cMo);
    sim.setDesiredCameraPosition(cdMo);
    sim.setExternalCameraPosition(cextMo);
    sim.setInternalCameraParameters(cam);
    sim.setExternalCameraParameters(cam);

    while (1)
    {

        /// Get the current pose of the camera
        robot.getPosition(wMc);

        /// Compute the pose of the object in the camera frame

        cMo = wMc.inverse() * wMo;

        vpColVector positionError = (cMo * cdMo.inverse()).getTranslationVector();

        /// Compute the pose of the four points on the object in the camera frame (tracking)

        for (unsigned int i = 0; i < 4; i++)
        {
            /// TODO Calculation of the new point position
            pointC.at(i) = changeFrame(pointO.at(i), cMo);
        }

        /// Compute the control law

                // New position of the four point in the image
        std::vector<vpColVector> pixels(4, vpColVector(2, 0));

        // Control gain
        double lambda = 1;

        // Visual error
        vpColVector error(8, 0);

        // Interaction matrix
        vpMatrix Le(8, 6, 0);
        vpColVector errorPixel(2, 0);
        for (unsigned int i = 0; i < 4; i++)
        {
            pixels.at(i) = projectPointOnImage(pointC.at(i), cam);

            vpMatrix L = computeInteractionMatrixOfPoint(pixels.at(i)[0], pixels.at(i)[1], cam, pointC.at(i)[2]);
            Le.insert(L, 2 * i, 0);


            /// TODO Calculation of the visual error on the pixel position
            errorPixel = pixels.at(i) - projectPointOnImage(changeFrame(pointO.at(i), cdMo), cam);
            error.insert(2 * i, errorPixel);
        }

        vpColVector vel(6, 0);
        /// TODO Calculation of the velocity to apply to the camera
        vpMatrix L_1; // L^-1
        L_1 = Le.pseudoInverse();
        vel = -lambda * L_1 * error;
   
/// Apply the control law to the camera

        robot.setVelocity(vpRobot::CAMERA_FRAME, vel);

/// Display

        sim.setCameraPositionRelObj(cMo);

        vpDisplay::display(Iint);
        vpDisplay::display(Iext);

        sim.getInternalImage(Iint);
        sim.getExternalImage(Iext);

        display_trajectory(Iint, pointC, cam);
        vpDisplay::flush(Iint);
        vpDisplay::flush(Iext);
        
        for(int i=0 ; i<3 ; i++)
        {
            velPlot.plot(0,i, time, vel[i]);
            velPlot.plot(1,i, time, vel[3+i]);
        }
        
        for(int i=0 ; i<4 ; i++)
        {
            errorPlot.plot(i,0, time, error[i]);
            errorPlot.plot(i,1, time, error[i+1]);
        }
        
        for(int i=0 ; i<3 ; i++)
        {
            positionErrorPlot.plot(0,i, time, positionError[i]);
        }

        // A click in the internal view to exit
        if(vpDisplay::getClick(Iint, false)) break;
        
        vpTime::wait(1000 * robot.getSamplingTime());
        time += 1000 * robot.getSamplingTime();
    }
} 
catch (vpException &e)
{
    std::cout << "Catch an exception: " << e << std::endl;
}
}

void display_trajectory(const vpImage<unsigned char> &I, std::vector<vpColVector> &point, const vpCameraParameters &cam)
{
    static std::vector<vpImagePoint> traj[4];
    vpColor color(128, 100, 50);
    vpImagePoint pixel;
    for(unsigned int i = 0; i < 4; i++)
    {
        // Project the point at the given camera position
        vpColVector proj = projectPointOnImage(point.at(i), cam);
        pixel.set_u(proj[0]);pixel.set_v(proj[1]);
        traj[i].push_back(pixel);
    }
    for(unsigned int i = 0; i < 4; i++)
    {
        for(unsigned int j = 1; j < traj[i].size(); j++)
        {
            vpDisplay::displayLine(I, traj[i].at(j-1), traj[i].at(j), color);
        }
    }
}

vpColVector changeFrame(const vpColVector &point, const vpHomogeneousMatrix &M)
{
    vpColVector newPoints(3,0);
    newPoints = M.getRotationMatrix() * point + M.getTranslationVector();
    return newPoints;
}

vpColVector projectPointOnImage(const vpColVector& point, const vpCameraParameters& cam)
{
    double u0 = cam.get_u0();
    double v0 = cam.get_v0();
    double f = cam.get_px();

    vpColVector pixel(2, 0);

    pixel[0] = (point[0]/ point[2]) * f + u0;
    pixel[1] = (point[1] / point[2]) * f + v0;
    
    return pixel;
}

vpMatrix computeInteractionMatrixOfPoint(double u, double v, const vpCameraParameters &cam, double Z)
{
    double u0 = cam.get_u0();
    double v0 = cam.get_v0();
    double f = cam.get_px();
    
    vpMatrix Ls(2,6,0);
    

    Ls[0][0] = -f / Z;
    Ls[0][1] = 0;
    Ls[0][2] = (u - u0) / Z;
    Ls[0][3] = (u - u0) * (v - v0) / f;
    Ls[0][4] = -f - (u - u0) * (u - u0) / f;
    Ls[0][5] = (v - v0);

    Ls[1][0] = 0;
    Ls[1][1] = -f / Z;
    Ls[1][2] = (v - v0) / Z;
    Ls[1][3] = f + (v - v0) * (v - v0) / f;
    Ls[1][4] = -(u - u0) * (v - v0) / f;
    Ls[1][5] = -(u - u0);

    return Ls;
}
