FROM debian:bookworm-slim
RUN apt update && apt install -y \
bash \
base \
gcc \
g++-12 \
cmake \
sudo

RUN echo 'root:root' | chpasswd
COPY user-init.sh /sbin/user-init
RUN /sbin/user-init
RUN chown -R dev:1000 /home/dev && chmod -R 755 /home/dev 
USER dev
COPY thirdparty/* /home/dev/BinMorph/thirdparty/
COPY CMakeLists.txt /home/dev/BinMorph/
ENTRYPOINT ["sleep","infinity"]