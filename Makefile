IMAGE_NAME = "example-dev-container"

.PHONY: run
run:
	docker run -it --rm -v $(PWD)/dev:/workspace $(IMAGE_NAME)

.PHONY: image
image:
	docker build -t $(IMAGE_NAME) .
