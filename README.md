# Architecture overview

```
Website (5781) --> UI
   |-------------> Envoy (5782) --> Backend (localhost:5783)
```
Port number indicated in parentheses.

# Backend

## Build
```
mkdir -p ./build
cd ./build
cmake ../backend
make
```

## Run
```
cd ./build
./server
```

## Clean
```
rm -rf ./build
```

# Frontend

## Development
```
cd ./frontend/web
npm run proto:generate
ng serve --host 192.168.1.10 --disable-host-check
```

## Build & deploy
```
cd ./frontend/web
make
make dist
```

## Run
```
cd ./frontend/envoy
envoy -c ./envoy.yaml

sudo systemctl restart apache2  # if necessary
```
