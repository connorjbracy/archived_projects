# Brief
This GitHub repository is paired
[a Docker repository](https://hub.docker.com/repository/docker/connorjbracy/archived_projects_containers)
used to store old projects that were created prior to my use of git (i.e., when
I was a student), but want to keep record of and have a way to interact with
going forward.

# Motivation
Since many of archived projects are written in C/C++, and I don't own
distribution rights to all files necessary to rebuild the projects (for many
projects, I was given some boilerplate code), rather than create a set of
pre-compiled binaries hoping to cover all environments, I decided to take each
project, load it onto a Docker image, compile the project for that environment,
remove the source, and upload the resulting image to this repository.

# Usage
Each project can be interacted with by running its corresponding Docker image,
which is set to start the pre-compiled project at start-up time. Many of the
projects are graphical in nature and these projects require an extra step to
interact with them. In the case of a graphical project, the image will dump some
output referring to a running `x11vnc` instance upon startup. After this output
completes, you will need to connect a VNC client to the IP Address allocated to
the running Docker container. There are likely other services which can
accomplish this, but I have only tested using `gvncviewer` on `Ubuntu 20.04`.
There is
[a convenience script](https://github.com/connorjbracy/archived_projects/blob/main/docker_template/vnc_connect.sh)
for accomplishing this in `docker_template` directory of the archived projects
repository. To use the script to connect to the Docker image, the Docker image
must be running and you should provide a single positional argument which is
the *tag* of the Docker image (i.e., what comes after the ":" in the image's
fullname)

For more details on a given project, please refer to the README in its
directory.

# Status

There are still many more project to be added, but doing so takes some time as
reviving things from the dead takes time. The projects were often written in
Visual Studio projects (and the Docker images run Ubuntu) and I'll be curating
files, writing briefs, etc. so there will be a continual rollout of projects
rather than uploading them all in one batch.
