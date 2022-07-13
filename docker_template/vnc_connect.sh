#!/usr/bin/env bash

VIEWER=${VIEWER:-"gvncviewer"}

function help_text () {
printf "\n\
Usage: %s [-v viewer] image\n\
\n\
Arguments:\n\
 image  - The name of the Docker image that contains the VNC server you are\n\
          trying to connect to.\n\
\n\
Options:\n\
 viewer - The name of the service that allows you to connect to the graphics\n\
          output of the docker image using the Docker image's IPAddress and\n\
          port '0'.\n\
          (default: %s)
\n" "$1" "$2"
}

# Get expected flag/option CLI arguments
while getopts v:-: OPT; do
  if [ "$OPT" = "-" ]; then
    OPT="${OPTARG%%=*}"
    OPTARG="${OPTARG#$OPT}"
    OPTARG="${OPTARG#=}"
  fi
  case "$OPT" in
    v | viewer ) VIEWER="$OPTARG" ;;
    ??* )
      echo "Unknown option --$OPT"
      exit 1
      ;;
    ? )
      echo "Case '?'"
      exit 2
      ;;
    * )
      echo "Case '*'"
      ;;
  esac
done
shift $((OPTIND-1))

# Get remaining positional CLI arguments
if [[ $# -gt 0 ]]; then
  DOCKER_IMAGE_NAME=$1
  shift
  # If there are any extra arguments, inform the user we didn't use them.
  if [[ $# -gt 0 ]]; then
    for (( i = 1; i <= $#; i++ )); do
      echo "Unknown positional argument '${!i}'"
    done
  fi
else
  SCRIPT_FULLNPATH=$(readlink -f "${BASH_SOURCE:-$0}")
  SCRIPT_FILENAME=$(basename "$SCRIPT_FULLNPATH")

  help_text "$SCRIPT_FILENAME" "$VIEWER"
  exit 3
fi

# Report back to the user what config values we have thus far
echo "Docker image name       = $DOCKER_IMAGE_NAME"
echo "Viewer                  = $VIEWER"

if [[ -z $(command -v "$VIEWER") ]]; then
  echo "Could not find viewer '$VIEWER' on this machine!"
  exit 4
fi

# Find the process ID of the Docker image
FOUND_DOCKER_IMAGE_ID=$(                              \
  docker ps                                           \
  | grep "$DOCKER_IMAGE_NAME"                         \
  | sed -r "s|^(\w+) +.+\b$DOCKER_IMAGE_TAG\b.+$|\1|" \
)
if [[ -z $FOUND_DOCKER_IMAGE_ID ]]; then
  echo "Could not find Docker image named '$DOCKER_IMAGE_NAME'!"
  exit 5
fi
echo "Found Docker image ID   = $FOUND_DOCKER_IMAGE_ID"

# Find the IP Address of the Docker container
DOCKER_IMAGE_IP_ADDRESS=$(                              \
  docker inspect "$FOUND_DOCKER_IMAGE_ID"               \
  | grep -m1 -P "\"IPAddress\""                         \
  | sed -r "s|^ *\"IPAddress\"\: *\"([0-9\.]+)\".*|\1|" \
)
echo "Docker image IP address = $DOCKER_IMAGE_IP_ADDRESS"

# Connect to the container to view the example program
DOCKER_IMAGE_IP_ADDRESS_PORT="$DOCKER_IMAGE_IP_ADDRESS:0"
echo "Connecting to Docker image using address $DOCKER_IMAGE_IP_ADDRESS_PORT..."
exec "$VIEWER" "$DOCKER_IMAGE_IP_ADDRESS_PORT"
