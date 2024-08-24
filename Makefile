IMAGE_NAME = "example-dev-container"
CONTAINER_NAME = $(IMAGE_NAME)

.PHONY: run
run: image
	docker run --name $(CONTAINER_NAME) -it --rm -v $(PWD)/dev:/workspace $(IMAGE_NAME)

.PHONY: image
image:
	docker build -t $(IMAGE_NAME) .
