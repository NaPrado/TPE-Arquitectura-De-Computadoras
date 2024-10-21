docker start TPE-Arqui
docker exec -u mateo -it TPE-Arqui make clean -C /root/Toolchain
docker exec -u mateo -it TPE-Arqui make clean -C /root/
docker exec -u mateo -it TPE-Arqui make -C /root/Toolchain
docker exec -u mateo -it TPE-Arqui make -C /root/
docker stop TPE-Arqui
