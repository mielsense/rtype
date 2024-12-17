#!/usr/bin/bash

kubectl apply -f r-type_server.configmap.yaml
kubectl apply -f r-type_master_server.deployment.yaml
kubectl apply -f r-type_server.deployment.yaml