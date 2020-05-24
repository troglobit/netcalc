FROM alpine:latest
RUN apk --no-cache add musl-dev git automake autoconf gcc make

COPY . /tmp/netcalc
RUN git clone --depth=1 file:///tmp/netcalc /root/netcalc
WORKDIR /root/netcalc

RUN ./autogen.sh && ./configure --prefix=/usr --disable-ipcalc-symlink && make -j5 && make install-strip

FROM alpine:latest
COPY --from=0 /usr/bin/netcalc /usr/bin/netcalc

CMD [ "/usr/bin/netcalc" ]
