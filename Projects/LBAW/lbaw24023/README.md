# SyncTask

**Project management done easier, keeping everyone up to date.**

> Database and Web Applications Laboratory (L.EIC023) 2024/2025  
> Bachelor in Informatics and Computing Engineering
>
> - Sérgio Sobral Nunes (Regent teacher)
> - Rúben Filipe Seabra de Almeida (Laboratory teacher)

---

- **Project Theme:** 7. Project Management
- **Group:** 24023
- **Authors:**
  - Duarte Souto Assunção, up202208319@up.pt
  - Guilherme Duarte Silva Matos, up202208755@up.pt
  - João Vítor da Costa Ferreira, up202208393@up.pt
  - Pedro Afonso Nunes Fernandes, up202207987@up.pt

---

### Installation

The final product is available as a Docker image in the [GitLab Container Registry](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/container_registry).  
To start the image, connect to the FEUP VPN, login and download the image:

```bash
$ docker login gitlab.up.pt:5050
$ docker run -d --name lbaw24023 -p 8001:80 gitlab.up.pt:5050/lbaw/lbaw2425/lbaw24023
```

### Credentials

| Type              | Email            | Password |
| ----------------- | ---------------- | -------- |
| Administrator     | admin@fe.up.pt   | password |
| Tailored account  | john@example.com | password |
| Generated account | user@fe.up.pt    | password |

### Wiki

- [ER: Requirements Specification](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/wikis/er)
- [EBD: Database Specification](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/wikis/ebd)
- [EAP: Architecture Specification and Prototype](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/wikis/eap)
- [PA: Product and Presentation](https://gitlab.up.pt/lbaw/lbaw2425/lbaw24023/-/wikis/pa)
