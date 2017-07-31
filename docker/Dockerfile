############################################################
# Dockerfile that builds and runs dependant docker files

FROM usd-docker/alusdmaya:centos6-usd-0.7.5

MAINTAINER Animal Logic

ENV TMP_DIR_STUDIOEXAMPLE /tmp/StudioExample
CMD mkdir -p $TMP_DIR_STUDIOEXAMPLE
COPY ./docker/build_studioexample.sh /tmp/
COPY ./ $TMP_DIR_STUDIOEXAMPLE
RUN /tmp/build_studioexample.sh
CMD rm -Rf $TMP_DIR_STUDIOEXAMPLE
CMD bash