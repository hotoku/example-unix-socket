	
FROM	ubuntu:24.04
RUN	apt-get update && apt-get install -y \
	build-essential
RUN	apt-get install -y \
	cmake
RUN	apt-get install -y \
	gdb
WORKDIR	/workspace
