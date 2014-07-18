#ifndef sensorflujo_h
#define sensorflujo_h
class sensorflujo {
  public:
      sensorflujo();
      double transformarMedida();
      double mostrarMedida();
      char * mostrarNombre();
      void rpm ();  
      void set_rpm();
    public:
      double flujo;
      char* _nombre;
      double revoluciones;
};
#endif
