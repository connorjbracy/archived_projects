# FROM ubuntu:focal-20220531
FROM ubuntu:latest
#RUN apt-get update && apt-get install -y firefox
RUN apt-get update && apt-get install -y x11vnc
RUN apt-get update && apt-get install -y xvfb
RUN apt-get update && apt-get install -y freeglut3-dev
RUN apt-get update && apt-get install -y mesa-utils
RUN apt-get update && apt-get install -y build-essential
RUN apt-get update && apt-get install -y clang
ARG LOCAL_SOURCE_DIR=.
ARG IMAGE_WORKING_DIR=/usr/local/etc
# COPY . /usr/local/etc
COPY $LOCAL_SOURCE_DIR $IMAGE_WORKING_DIR
RUN clang++ -g -Wall -lglut -lGL -lGLU -I$IMAGE_WORKING_DIR $IMAGE_WORKING_DIR/cs200glut_demo.cpp -o $IMAGE_WORKING_DIR/out -v
# RUN echo "exec firefox" > ~/.xinitrc && chmod +x ~/.xinitrc
RUN echo "exec $IMAGE_WORKING_DIR/out" > ~/.xinitrc && chmod +x ~/.xinitrc
# CMD ["x11vnc", "-ncache", "10", "-create", "-forever"]
CMD ["x11vnc", "-create", "-forever"]
