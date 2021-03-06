//----------------------------------------------------------------------------//
//                                                                            //
// This file is part of the fisheye stitcher project.                         //
// Copyright (c) 2018-2020 Tuan Phan Minh Ho <drnoob2013@gmail.com>           //
// https://github.com/drNoob13/fisheyeStitcher                                //
//                                                                            //
//----------------------------------------------------------------------------//
#include "fisheye_stitcher.hpp"
#include "input_parser.hpp"

int
main(int argc, char **argv)
{
    // Parse input 
    ffstitcher::InputParser Parser(argc, argv);

    // Video input 
    cv::VideoCapture VCap( Parser.getVideoPath() );
    if( !VCap.isOpened() )
    {
        CV_Error_(cv::Error::StsBadArg, ("Error opening video: %s",
                    Parser.getVideoPath().c_str()));
    } 

    // Video output 
    int    frame_fps      = VCap.get(CV_CAP_PROP_FPS);
    int    frame_width    = VCap.get(CV_CAP_PROP_FRAME_WIDTH);
    int    frame_height   = VCap.get(CV_CAP_PROP_FRAME_HEIGHT);

    CV_Assert( (frame_width % 2 == 0) && (frame_height % 2 == 0) );

    std::string video_out_name = Parser.getOutDir() + "/" + 
                        Parser.getImageName() + "_blend_video.avi";

    int Wd = static_cast<int>(frame_width / 2 * 360.0 / MAX_FOVD);
    int Hd = static_cast<int>(Wd / 2);
    Hd = Hd - (Hd % 10); // trim exceesive pixel lines
    cv::VideoWriter VOut;
    VOut.open( video_out_name, cv::VideoWriter::fourcc('X','2','6','4'), // OpenCV 4.1.0
               frame_fps, cv::Size(Wd, Hd) );
    if( !VOut.isOpened() )
    {
        CV_Error_(cv::Error::StsBadArg, 
                  ("Error opening video: %s", video_out_name.c_str()));
    } 

    // Create a stitcher
    stitcher::FisheyeStitcher Stitcher(
        frame_width, 
        frame_height,
        195.0f,
        Parser.getFlagLightCompen(),
        Parser.getFlagRefineAlign(),
        Parser.getMLSMapPath()
    );

    // Main video loop
    int count = 0;
    cv::Mat img, frame;

    std::cout << "Starting frame stitching..\n";

    double startTime, endTime;
    double totalTime;
    double total_stitch_time = 0;
    startTime = double(cv::getTickCount()); // frame stitching starts 

    while( 1 )
    { 
        VCap >> img;

        if( img.empty())
        {
            std::cout << "end of video\n";
            break;
        }
 
        cv::Mat img_l, img_r;
        img_l = img(cv::Rect(0,  0, int(img.size().width / 2), 
                    frame_height)); // left fisheye
        img_r = img(cv::Rect(int(img.size().width / 2), 0, 
                    int(img.size().width / 2), frame_height)); // right fisheye 

        double stitch_start = static_cast<double>(cv::getTickCount()); // frame stitching starts

        // Stitch video frames
        cv::Mat pano;
        pano = Stitcher.stitch(img_l, img_r);

        double stitch_end = static_cast<double>(cv::getTickCount());
        total_stitch_time += ((stitch_end - stitch_start) / cv::getTickFrequency());

        if( count % 30 == 0 )
        {
            std::cout << "Stitching frame: " << count << "\n";
        }

#if PROFILING
        double tickStart = endTime; // previous count
#endif 
        VOut << pano;

#if PROFILING
        double tickEnd = static_cast<double>(cv::getTickCount());
        double runTime = (tickEnd - tickStart) / cv::getTickFrequency();
        tickStart = tickEnd;
        std::cout << "run-time (resize) = " << runTime << " (sec)" << "\n";
#endif 
        count++;

    }   // while(1)

    // RunTime
    endTime = static_cast<double>(cv::getTickCount());
    totalTime = (endTime - startTime) / cv::getTickFrequency();

    VCap.release();
    VOut.release();

    std::cout << "Done! Writing to video [" << Wd << "x" << Hd << "] @" 
         << frame_fps << "fps  --> " << video_out_name << "\n";

    std::cout << "Total time = " << totalTime / 60 << " min" 
              << " (" << totalTime << " sec)\n";
    std::cout << "Total stitching time = " << total_stitch_time / 60 << " min" 
              << " (" << total_stitch_time << " sec)\n";
    std::cout << "Average frame time = " << totalTime / (count*60) << " min" 
              << " (" << totalTime / count << " sec)\n";
    std::cout << "Average frame stitch time = " << total_stitch_time / (count*60) << " min" 
              << " (" << total_stitch_time / count << " sec)\n";

    std::cout << "\nSee you again.\n";

    return 0;
}