
show_help() {
   echo "Usage: $0 [OPTIONS]"
   echo ""
   echo "Options:"
   echo "  --name NAME        Specify the name"
   echo "  --domain DOMAIN    Specify the domain"
   echo "  --version VERSION  Specify the android version (34 by default)"
   echo "  --help             Show this help message"
}

parse_flags() {
  PROJECT_NAME=""
  DOMAIN=""
  VERSION=""

  while [[ $# -gt 0 ]]; do
    case "$1" in
      --name)
        if [[ -n "$2" && "$2" != --* ]]; then
          PROJECT_NAME="$2"
          shift 2
        else
          echo "Error: --name requires a non-empty argument."
          exit 1
        fi
        ;;
      --domain)
        if [[ -n "$2" && "$2" != --* ]]; then
          DOMAIN="$2"
          shift 2
        else
          echo "Error: --domain requires a non-empty argument."
          exit 1
        fi
        ;;
      --version)
        if [[ -n "$2" && "$2" != --* ]]; then
          VERSION="$2"
          shift 2
        else
          echo "Error: --version requires a non-empty argument."
          exit 1
        fi
        ;;
      --help)
        show_help
        exit 0
        ;;
      *)
        echo "Unknown option: $1"
        show_help
        exit 1
        ;;
    esac
  done

  if [[ -z "$PROJECT_NAME" || -z "$DOMAIN" || -z "$VERSION" ]]; then
    echo "Error: Missing required arguments."
    show_help
    exit 1
  fi
}
