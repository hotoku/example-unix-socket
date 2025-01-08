FROM node:18.20.4

COPY src src
COPY package.json package.json
COPY package-lock.json package-lock.json

RUN npm install

ENTRYPOINT ["npm", "run"]
