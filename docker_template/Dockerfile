FROM ubuntu:focal-20220531

RUN apt-get update && apt-get install -y x11vnc
RUN apt-get update && apt-get install -y xvfb
RUN apt-get update && apt-get install -y freeglut3-dev
RUN apt-get update && apt-get install -y mesa-utils
RUN apt-get update && apt-get install -y build-essential
RUN apt-get update && apt-get install -y clang

ARG LOCAL_LIB_DIR=./lib
ARG LOCAL_SRC_DIR=./src

ARG IMAGE_WORKING_DIR=/usr/local/etc
ARG IMAGE_LIB_DIR=$IMAGE_WORKING_DIR/lib
ARG IMAGE_SRC_DIR=$IMAGE_WORKING_DIR/src

# COPY . /usr/local/etc
COPY $LOCAL_LIB_DIR $IMAGE_LIB_DIR
COPY $LOCAL_SRC_DIR $IMAGE_SRC_DIR
# RUN echo $IMAGE_WORKING_DIR
# RUN echo $IMAGE_LIB_DIR
# RUN echo $IMAGE_SRC_DIR
# RUN echo $PATH
ENV PATH="${IMAGE_LIB_DIR}:$PATH"
# RUN echo $PATH

# RUN clang++ -g -Wall -lglut -lGL -lGLU -I$IMAGE_WORKING_DIR $IMAGE_WORKING_DIR/cs200glut_demo.cpp -o $IMAGE_WORKING_DIR/out -v
RUN clang++ -g -Wall -lglut -lGL -lGLU -I$IMAGE_LIB_DIR $IMAGE_SRC_DIR/cs200glut_demo.cpp -o $IMAGE_WORKING_DIR/out -v
RUN rm -r $IMAGE_SRC_DIR

# RUN echo "exec $IMAGE_WORKING_DIR/out" > ~/.xinitrc && chmod +x ~/.xinitrc
RUN echo "exec ${IMAGE_WORKING_DIR}/out" > ~/.xinitrc && chmod +x ~/.xinitrc

CMD ["x11vnc", "-create", "-forever"]
