FROM debian:bullseye-slim

# Install dependencies for C and C++ development
RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    git \
    apache2-utils \
    libcriterion-dev \
    && rm -rf /var/lib/apt/lists/*

# Create user and group
RUN groupadd -r brice && useradd --no-log-init -r -g brice brice

# Set up sudo
RUN apt-get update && apt-get install -y sudo && rm -rf /var/lib/apt/lists/*
RUN echo "brice ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/brice

# Set up the user's home directory
RUN mkdir -p /home/brice && chown -R brice:brice /home/brice

# Set up the user's workspace
RUN mkdir -p /workspace && chown -R brice:brice /workspace
