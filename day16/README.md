vis.cpp generates frame%04d.ppm with all the shortest paths. Combine with ffmpeg to create a movie:
```
ffmpeg -r 60 -f image2 -i frame%04d.ppm -vcodec libx264 -crf 25 -vf scale=1080:1080:flags=neighbor,pad=1920:1080:-1:-1 output.mp4
```
