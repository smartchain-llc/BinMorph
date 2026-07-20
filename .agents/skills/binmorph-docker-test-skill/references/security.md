# Docker Test Security

Prohibited by default:

- mounting `/var/run/docker.sock`;
- `--privileged`;
- host PID, IPC, or network namespace;
- mounting the entire host home directory;
- passing secrets through Dockerfile `ARG` or `ENV`;
- running project-controlled tests as root;
- downloading and executing unpinned scripts during image build.

Use BuildKit secret and SSH mounts only for unavoidable authenticated dependency access, and ensure secret data is absent from final layers and logs.

For adversarial schema or binary tests, apply memory, CPU, process, and file-size limits. A test container is not itself a sufficient sandbox when it has dangerous host capabilities.
