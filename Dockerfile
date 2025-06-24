# syntax=docker/dockerfile:1.4
FROM --platform=linux/arm64 drogonframework/drogon:latest

WORKDIR /app

COPY . .

RUN mkdir build && \
    cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make -j$(nproc)

ENV DROGON_CONFIG_FILE=/app/config.json

EXPOSE 9001

CMD [ "./build/nas_backend", "--config", "/app/config.json" ]