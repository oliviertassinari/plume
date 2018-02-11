
gst-launch-0.10 -v souphttpsrc location='http://<ip>:8080/video' is-live=true ! multipartdemux ! decodebin2 ! v4l2sink device=/dev/video1

vlc v4l2:///dev/video0

git clone https://github.com/umlaeute/v4l2loopback.git

make

sudo make install

sudo make modprobe

sudo modprobe v4l2loopback