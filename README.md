# Proyecto 1 IIC2333
Diego Collao Alcayaga (dlcollao@uc.cl)
Tomás Flores Mella (tfflores@uc.cl)
Daniel Maltrain Pizarro (dtmaltrain@uc.cl)
Fernando Pérez Masri (fperezm1@uc.cl)


Para acceder a cualquier entrega diríjase a su carpeta respectiva.

## Decisiones de diseño:
Se 

## Supuestos
Caca

## Anexo: Informe de códigos de Error (Bonus)

Nuestro manejo de errores abarca siete códigos posibles. Si se produce un error durante la ejecución, esta no se interrumpe y en cambio se imprime en pantalla el contenido asociado al código que corresponda antes de que finalice el programa. Los códigos están numerados del 0 al 6, y son:

- Código 0: Valor por defecto. Si el programa finaliza y la variable global sigue guardando este código, es porque no se han producido errores. 

    Mensaje asociado: <em>Finished without errors</em>

- Código 1: La primera acción a realizar es la de montar el disco. Por ende, si se llama a alguna función que requiera ocupar el disco virtual antes de que esté montado, se producirá este error. 

    Mensaje asociado: <em>Attempted action requires disk to be mounted</em>

- Código 2: Aparece cuando se intenta borrar una partición con <em>id</em> mayor al máximo posible. 

    Mensaje asociado: <em>ID out of range</em>

- Código 3: Cuando se llama a la función que imprime el bitmap, si se le pasa un número de bloque mayor al máximo de bloques del bitmap, aparecerá este error.

    Mensaje asociado: <em>Bitmap ID out of range</em>

- Código 4: Este ocurre si se intenta montar el disco en una partición de <em>id</em> mayor al máximo posible.

    Mensaje asociado: <em>Partition number out of range</em>

- Código 5: Se produce cuando se intenta abrir un archivo que ya existe en modo <em>"w"</em> con <em>os_open</em>.

    Mensaje asociado: <em>Tried to write a file that already exists</em>

- Código 6: Este aparece durante la creación de una partición. Surge si se cuenta con un <em>size</em> negativo, o bien cuando la <em>id</em> especificada corresponde a una partición ya existente.

    Mensaje asociado: <em>Partition cannot be created</em>
