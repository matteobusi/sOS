
#include <graphics.h>
#include <stdout.h>
//FROM GNU Multiboot specification
#include <multiboot.h>

#include <system.h>

static void print_offset(int n)
{
    set_color(0,7);
    int i;
    for(i=0; i  < n; i++)
        kprintf(" ");
    set_color(7,0);
}

extern void print_logo()
{
    /*char logo[][82]={"        OMMMMMMMMMMMMZ$                                   MMMMMMMMMMMMMMMMMMZ\n" ,
                     "   MMMMMMMMMMMNOO8MMMD$          MMNZ       ZZ        OMMMMMMMMMMMN8OO8MMMMOZ\n" ,
                     " $DMMMMMMMNOOZOD8OD8MM8$        OMMMO8    8$MOZ      MMMMMMM8OOOO8NDD88ONOOOZ\n" ,
                     "8MMMMMMOOZ8O       $NNZO       MMMMO8     NMMO$    MMMMM8OOODO$       ZODO   \n" ,
                     "DMMMOOZ8           ONZ8       MMMMOD      OMOZ    NNMMOO8O$         ZZ8      \n" ,
                     "OOOZO             MNZ8      DMMMNO8      ZZNZ$    NMMO8Z           Z         \n" ,
                     "                 MDZ8      MMMMNZ8       MMO8    ONMMNDO                     \n" ,
                     "              NMOZO     OMMMMNO8        MOZ     ZONMMMD 8DND                 \n" ,
                     "             MMOZ$     MMMMMNZ8        MOO$      ODZOOOODMMM8 Z8DNZ          \n" ,
                     "           8MNO8     DOONMMNO8        M8O$          $ZO8DN8ZOOOONMN$ND       \n" ,
                     "          MMDZD     NO8N$MMO8        MDOO                   $ZODDOOOMDDZ     \n" ,
                     "         MMOZO    DOOO ZMMO8       DMOOZ                          $88DMO     \n" ,
                     "       OMMOO$    NOO  DZMOZ       MMOZ$                              ZMOZ    \n" ,
                     "    $MNMNOD    MOO8  ZZM8Z      OMMOO$                              NMMO$    \n" ,
                     "   ZMMMMOO   MMOO$   N$MOZ     MMDOD           O8                DMMMMOO     \n" ,
                     "   N8MMMM  MMOO8     NMM8    MMMOOO        $ MMD$          DMMMMMMMMOOD      \n" ,
                     "  $8DMMMMMMNOOZ      DMMMMMMMMOOD$        Z MMMMMMMMMMMMMMMMMMMMM8OOD$       \n" ,
                     "   $ODMMMNOOO        8ONMMMMOO8Z          8NMMMMMMMMMMMMMMMM8OOOODO          \n" ,
                     "   $$ZZOOO8           OOZOOODZ            Z8ZOOOOOOOOOOOOOODD8Z              \n" ,
                     "                   ***** Codename: VoxOS * Step 1.7 *****                    \n",
                     "                           Released under GNU GPL2                           \n"};*/
    char logo[][50]= {" *** SoS * Step 2.1 ***\n",
                      "Building a basic multitasking OS based\n",
                    "on Round-Robin Scheduling with priorities\n",
                      "Released under GNU GPL2\n"};
    int i;
    for(i=0; i < 4; i++)
    {
        print_offset((COLUMNS-strlen(logo[i]))/2);
        kprintf(logo[i]);
    }
    set_color(0,7);
}

void memory_info(void* m_boot_addr)
{
    extern unsigned int kend;
    multiboot_info_t* mboot_ptr = (multiboot_info_t*) m_boot_addr;
    //Shows where the kernel ends
    set_color(0, 9);
    kprintf("*** Kernel ends at: ");
    set_color(0, 7);
    kprintf("%d\n", &kend);
    //Shows informations about memory
    set_color(0, 9);
    kprintf("*** Lower Memory (KB): ");
    set_color(0, 7);
    kprintf("%d **", mboot_ptr->mem_lower);
    
    set_color(0, 9);
    kprintf(" Upper Memory (KB): ");
    set_color(0, 7);
    kprintf("%d ***\n", mboot_ptr->mem_upper);
}
void init_effect(char* text)
{
    int i;
    kprintf("%s ", text);
    for(i=0; i < 10; i++)
        kprintf("=");
    kprintf("> [");
    set_color(0, 9);
    kprintf("DONE");
    set_color(0, 7);
    kprintf("]\n");
}