
# Trabalho realizado nas Semanas #2 e #3

## Identificação

- CVE ID: CVE-2024-45063;
- Malicious software running in a guest VM that exposes virtio_scsi can exploit the vulnerabilities to achieve code execution on the host [4];
- A malicious iSCSI initiator could achieve remote code execution on the iSCSI target host;
- Affected Systems [3]:
    - FreeBSD 14.1-RELEASE before p4;
    - FreeBSD 14.0-RELEASE before p10;
    - FreeBSD 13.3-RELEASE before p6.

## Catalogação

- This vulnerability was reported in September 4th 2024 by Synacktiv [1];
- CWE-416: Kernel Use-After-Free [2];
- Severity:
    - NVD: "8.8 High" base score (CVSS 3.x);
    - CISA-ADP:  "9.8 Critical" base score (CVSS 3.x).

## Exploit

- No exploit reported (No CISA KEV) [5];
- No type of automation of this exploit has been found (e.g. Metasploit);
- Type of exploit: Memory corruption;
- Since VMs are executed with privileges, an attacker could have root access to the host and run arbitrary code.

## Ataques

- No known successful attacks until now;
- This vulnerability has the potential to access root privileges on the host machine, allowing for arbitrary code execution;
- Despite that, there is a possible way to attack: having a virtual machine running on the bhyve hypervisor and using the ctld iSCSI daemon just write to said buffer and call the function ctl_write_buffer [1].

## Referências
[1] https://www.freebsd.org/security/advisories/FreeBSD-SA-24:11.ctl.asc  
[2] https://nvd.nist.gov/vuln/detail/CVE-2024-45063  
[3] https://www.cve.org/CVERecord?id=CVE-2024-45063  
[4] https://www.tenable.com/cve/CVE-2024-45063  
[5] https://debricked.com/vulnerability-database/vulnerability/CVE-2024-45063