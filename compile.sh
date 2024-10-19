docker start TP
docker exec -u nahuel-prado -it TPArqui make clean -C /root/Toolchain
docker exec -u nahuel-prado -it TPArqui make clean -C /root/
docker exec -u nahuel-prado -it TPArqui make -C /root/Toolchain
docker exec -u nahuel-prado -it TPArqui make -C /root/
docker stop TP