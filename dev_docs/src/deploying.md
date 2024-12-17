# Summary

[- Deploy with Docker](#r-type-server-docker-setup)

[- Deploy with kubernetes](#r-type-server-kubernetes-setup)


# R-Type Server Docker Setup

This repository provides a Docker setup for building and running the R-Type game server. It utilizes multi-stage builds to create both the game server (`r-type_server`) and the master server (`r-type_master_server`) using a custom C++ project.

## Prerequisites

Before you start, ensure you have the following installed on your machine:

- [Docker](https://www.docker.com/)

## Dockerfile Overview

The Dockerfile is structured into several stages:

1. **Base Stage**: Based on `ubuntu:latest`, installs essential packages such as Git, Curl, CMake, and others needed for the build process.

2. **Build Stage**: Clones the Microsoft vcpkg repository and bootstraps it, setting up dependencies for C++ projects.

3. **Compile Stage**: Clones the R-Type project from GitHub, installs required system libraries, and compiles the server binaries (`r-type_server` and `r-type_master_server`).

4. **Server Image Stage**: Creates two minimal Alpine-based images, one for `r-type_server` and one for `r-type_master_server`, copying the compiled binaries and setting them as the entry point for the respective containers.

## Build the R-Type Server Image

To build the `r-type_server` image, you need to run the following command (must be run in the root folder):

```bash
sudo docker build . -t godlyjaaaaj/r-type_server --target r-type_server
```

This command performs the following action:
- Builds the `r-type_server` binary inside a container and packages it into a lightweight Alpine image.

## Build the R-Type Master Server Image

To build the `r-type_master_server`, use a similar command:

```bash
sudo docker build . -t godlyjaaaaj/r-type_master_server --target r-type_master_server
```

This will compile and package the master server.

## Running the Servers

Once the images are built, you can run the servers with the following commands:

### Running the R-Type Server

```bash
sudo docker run -d --name r-type_server godlyjaaaaj/r-type_server [ARGS...]
```

This command will start the game server in detached mode.

### Running the R-Type Master Server

```bash
sudo docker run -d --name r-type_master_server godlyjaaaaj/r-type_master_server [ARGS...]
```

This command will start the master server in detached mode.

## Additional Notes

- **Multi-Stage Build**: The multi-stage build approach ensures that the final server images are as small as possible, by only including the necessary binaries and runtime dependencies.

## Troubleshooting

If you encounter any issues while building or running the servers, here are a few things to check:

1. **Build Errors**: Check the build logs for missing dependencies or configuration issues.
2. **Docker Daemon**: Ensure the Docker daemon is running and that you have sufficient permissions.

# R-Type Server Kubernetes Setup

## Prerequisites

Before you start, ensure you have the following tools:
- [kubectl](https://kubernetes.io/docs/tasks/tools/)
- a running Kubernetes cluster
- already built Docker images for the R-Type server and master server and pushed them to an image registry

## Deploying the R-Type Server

**The repo contains 3 Kubernetes manifests:**
- `r-type_server.deployment.yaml`: Deploys the R-Type server as a Kubernetes deployment.
- `r-type_master_server.deployment.yaml`: Deploys the R-Type master server as a Kubernetes deployment.
- `r-type_server.configmap.yaml`: Contains the configuration for the R-Type server.
 
**To deploy the R-Type server, run the following command:**

```bash
kubectl apply -f r-type_server.configmap.yaml
kubectl apply -f r-type_master_server.deployment.yaml
kubectl apply -f r-type_server.deployment.yaml
```

or you can use the `deploy_on_kube.sh` script which basically does the same thing.

```bash
./kube/deploy_on_kube.sh
```

## Additional Notes

- Right now there is no service exposed for the R-Type server, so you will need to port-forward the server to access it or create your own service.
