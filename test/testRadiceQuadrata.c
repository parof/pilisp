/**
 * \addtogroup test
 */
int main(int argc, char *argv[]){
    double numero = atof(argv[1]);
    double radice = atof(argv[2]);
    return !(radiceQuadrata(numero) == radice);
}