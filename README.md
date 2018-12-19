# fisheyeStitcher
Stitch images generated by dual-fisheye cameras.

[<p align="center"><img src="https://github.com/drNoob13/fisheyeStitcher/blob/master/misc/clip.gif" alt="drawing" width="500"/></p>](https://youtu.be/GtZF6EKe50U)


If you find our code useful, please consider citing our following papers:

```
[1]  T. Ho, I. D. Schizas, K. R. Rao and M. Budagavi, "360-degree video stitching for dual-fisheye lens cameras based on rigid moving least squares," 2017 IEEE International Conference on Image Processing (ICIP), Beijing, China, Sept. 2017, pp. 51-55.
[2]  T. Ho and M. Budagavi, "Dual-fisheye lens stitching for 360-degree imaging," 2017 IEEE International Conference on Acoustics, Speech and Signal Processing (ICASSP), New Orleans, LA, Mar. 2017, pp. 2172-2176.
```

## Installation

The code is built in Ubuntu 18.04 LTS (but should work fine under Ubuntu 16.04 LTS as well).

### Pre-requiste

* OpenCV 3.4.2 (with calib3d), which I built from source.
* ffmpeg `sudo apt-get install ffmpeg`, which is used to combined stitched frames and playback.
* cmake ``.

### Build

* Clone the repo and go to the cloned repo.

* Build the code
```
cmake .
make
```

* Run the code with provided sample images
```
./RUN_fisheye.sh
```

Please be informed that the code doesn't include the temporal coherence control, but one can implement it using the description in [1].


## Performance

It takes around 70ms-90ms to stitch one 3840x1920 image (02 x fisheye images) from Gear360 C200 on a laptop with an Intel i7-8750H CPU + 32GB Memory. See [clip](https://youtu.be/GtZF6EKe50U).
