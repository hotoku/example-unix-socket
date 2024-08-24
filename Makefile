IMAGE_NAME := "example-socket"
CONTAINER_NAME := $(IMAGE_NAME)
WORKDIR := /workspace

.PHONY: run
run: image
	docker run --name $(CONTAINER_NAME) -it --rm -v $(PWD):$(WORKDIR) $(IMAGE_NAME)

.PHONY: image
image:
	docker build --build-arg WORKDIR=$(WORKDIR) -t $(IMAGE_NAME) .
