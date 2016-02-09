/**
 * \file ESDL.h
 * \brief EasySDL header
 * \author TAHRI Ahmed, SIMON JÃ©rÃ©my
 * \version 0.7.0
 * \date 01-03-2015
 *
 * EasySDL est une extension de la librairie SDL standard
 *
 */

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fmodex/fmod.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <sqlite3.h>

typedef enum {

	ALIGN_CENTER = 1, /*!< Texte alignÃ© au centre */
	ALIGN_LEFT, /*!< Texte alignÃ© Ã  gauche */
	ALIGN_RIGHT /*!< Texte alignÃ© Ã  droite */

} t_typeAlign;

typedef enum {

	NUMERIC, /*!< Capture uniquement les chiffres de 0 Ã  9 */
	ALPHA, /*!< Capture uniquement les caractÃ¨res de A Ã  Z */
	ALPHANUMERIC, /*!< Capture uniquement les caractÃ¨res alphanumÃ©riques */
	NOMASK, /*!< Capture tout, aucun masque de saisie */
	NONE /*!< Lorsqu'il ne s'agit pas d'un champ de saisie */

} t_typeForm;

typedef enum {

	BUTTON, /*!< Les boutons interactifs */
	INPUT, /*!< Les champs de saisie */
	IMG, /*!< Les images */
	SPRITE, /*!< Les sprites */
	TEXT, /*!< Le texte */
	RECTANGLE /*!< Rectangles */

} t_typeData;

/**
 * \struct t_object
 * \brief DÃ©crit de quoi est fait un objet
 * 
 * t_object dÃ©crit un bouton ou un champs de saisie
 */

typedef struct {

	t_typeData type;
	int x;
	int y;
	char title[50];
	int align;
	char * dest; //Only for form obj
	t_typeForm typeForm; //Only for form obj
	int MouseOver; // 1 = Mouse is over, 0 = Not over..
	SDL_Surface *buffer_title, *buffer_content;
	int * id;
	int z_index;

} t_object;

/**
 * \struct t_rect
 * \brief DÃ©crit un rectangle simple avec une couleur
 *
 * t_rect contient les donnÃ©es nÃ©cessaires au dessin d'un rectangle
 */
typedef struct {

	SDL_Rect def;
	SDL_Color color;
	int *id;
	int z_index;

} t_rect;

/**
 * \struct t_text
 * \brief Contient une ligne de texte pour un rendu SDL
 *
 * t_text dÃ©crit une chaine de caractÃ¨re Ã  transformer en SDL_Surface, ne supporte pas les caractÃ¨res de type '\\x' !
 */
typedef struct {

	SDL_Color couleur;
	char * content;
	int x;
	int y;
	SDL_Surface *buffer;
	int * id;
	int z_index;

} t_text;

/**
 * \struct t_image
 * \brief Une image et ses paramÃ¨tres
 *
 * t_image contient la SDL_Surface de une image ainsi que ses paramÃ¨tres pour le rendu.
 */
typedef struct {

	char * file;
	int x; /*!< Position x relative au contexte parent */
	int y; /*!< Position y relative au contexte parent */
	SDL_Surface *buffer; /*!< La surface SDL de l'image */
	int * id;
	int z_index;
	
} t_image;

/**
 * \struct t_audio
 * \brief Un son avec le nom fichier associÃ©
 *
 * t_audio contient un son chargÃ© et le nom de fichier associÃ© Ã  celui-ci
 */
typedef struct {

	char * file; /*!< Nom du fichier audio associÃ© */
	FMOD_SOUND * buffer; /*!< Buffer du fichier audio chargÃ© */

} t_audio;

/**
 * \struct t_sprite
 * \brief Une image sprite et ses paramÃ¨tres
 *
 * t_sprite contient les paramÃ¨tres utiles Ã  la mise en place d'un sprite (ex: personnage animÃ©).
 */
typedef struct {

	char * file;
	SDL_Surface *buffer; /*!< Buffer de l'image sprite utile */
	SDL_Color transparancy; /*!< Couleur clÃ© pour la transparence */
	int sp_height, sp_width;
	int x, y;
	int position, animation;
	int hide; /*!< Indique si le sprite doit Ãªtre afficher */
	int z_index;

} t_sprite;

/**
 * \struct t_layer
 * \brief gestion des calques
 *
 * t_layer contient la position d'un t_typeData dans la profondeure.
 */
typedef struct {

	t_typeData type;
	int idObj;
	int z_index;

}t_layer;

/**
 * \struct t_context
 * \brief ReprÃ©sentation d'un contexte et son rendu.
 *
 * t_context contient un ensemble de donnÃ©e pour la formation d'une frame dans un Ã©tat donnÃ©e.
 */
typedef struct {

	char * title;
	SDL_Surface * contextSurface;

	t_object * contextObj;
	int nbObj;

	t_text * contextText;
	int nbText;

	t_image * contextImg;
	int nbImg;

	t_sprite * contextSprite;
	int nbSprite;

	t_rect * contextRect;
	int nbRect;

	SDL_Rect * updatedZones;
	int nbZone;

	int x, y;
	int height, width;

	t_layer ** contextSet;
	int nbSet;

	int * nbLayer;

} t_context;

/**
 * \struct Input
 * \brief Contient le mapping des Ã©vÃ¨nements liÃ©s au clavier et la souris.
 *
 * L'Ã©tat des touches / souris Ã  un instant t.
 */
typedef struct
{

	char key[SDLK_LAST];
	int mousex,mousey;
	int mousexrel,mouseyrel;
	char mousebuttons[8];
    char quit;

} Input;

/**
 * \struct d_param
 * \brief Contient un couple (ParamÃ¨tre, Valeur)
 *
 * Un couple indisociable dont la valeur est cryptÃ©
 */
typedef struct {

	char *param;
	unsigned char *value;

} d_param;

/**
 * \struct d_save
 * \brief DÃ©crit un jeu de donnÃ©es
 *
 * Contient les couples (paramÃ¨tre, valeur) et le nom du fichier Ã  manipuler.
 */
typedef struct {

	char * filename;
	d_param * data;
	int elem;

} d_save;

/**
* \fn void SDL_initWindow(int width, int height, int fullscreen, char * title, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support)
* \brief Initialise les modules de base et ouvre une fenÃªtre SDL
*
* \param width Largeur de la fenÃªtre SDL
* \param height Hauteur de la fenÃªtre SDL
* \param fullscreen Plein Ã©cran = 1, fenÃªtre = 0.
* \param title Titre de la fenÃªtre SDL
* \param icon_name Chemin vers l'icone de l'application sinon NULL pour aucune icone
* \param ttf_support Charge le module de rendu texte = 1 sinon 0
* \param police_name Nom du fichier *.ttf pour le moteur de rendu texte
* \param police_size Taille en pt de la police d'Ã©criture
* \param audio_support Prend en charge l'audio avec fmodex = 1 sinon 0
* \return void
*/
void SDL_initWindow(int width, int height, int fullscreen, char * title, char * icon_name, int ttf_support, char * police_name, int police_size, int audio_support);

/**
* \fn void SDL_unload()
* \brief DÃ©charge la mÃ©moire des modules prÃ©cÃ©demment chargÃ©s et ferme le processus
*
* \return void
*/
void SDL_unload();

/**
* \fn int SDL_generateMenu(char * backgroundPic, int nbEntries, char ** captions)
* \brief GÃ©nÃ¨re un menu gÃ©nÃ©rique et attends que l'utilisateur fasse un choix
*
* \param backgroundPic Nom de l'image Ã  utiliser en fond
* \param nbEntries Nombre de bouton Ã  gÃ©nÃ©rer
* \param captions Contient le texte associÃ©s au(x) bouton(s) (matrice **char)
* \return Identifiant du bouton choisis
*/
int SDL_generateMenu(t_context * menu, char * backgroundPic, int nbEntries, char ** captions);

/**
* \fn int SDL_ismouseoverArea(t_context * context, int height, int width, int x, int y)
* \brief VÃ©rifie si la souris est dans la zone dÃ©finie en paramÃ¨tre
*
* \param context Contexte de rÃ©fÃ©rence concernÃ©
* \param height Hauteur de la zone en pixel
* \param width Largeur de la zone en pixel
* \param x CoordonnÃ©e x du point supÃ©rieur gauche
* \param y CoordonnÃ©e y du point supÃ©rieur gauche
* \return bool
*/
int SDL_ismouseoverArea(t_context * context, int height, int width, int x, int y);

/**
* \fn int SDL_ismouseover(t_context * context, t_typeData type)
* \brief Renvoie l'identifiant d'un objet (bouton, champ de saisie, image, sprite, texte) sur lequel la souris survol le-dit objet.
*
* \param context Le contexte pour lequel il faut vÃ©rifier
* \param type Le type Ã  vÃ©rifier
* \return Identifiant de l'objet ou -1 si rien.
*/
int SDL_ismouseover(t_context * context, t_typeData type);

/**
 * Détermine si en plein écran ou non
 * @return Retourne 1 si en plein écran sinon 0
 */
int SDL_isFullScreen();

void SDL_printLayer(t_context * context);
/**
 * Ajoute un calque
 * \param  context Contexte concerné
 * \param  type    Type de l'objet
 * \param  idObj   Identifiant de l'objet
 * \param  z_index Numéro de calque
 * \return         Retourne 1 si réussi sinon 0
 */
int SDL_addLayer(t_context * context, t_typeData type, int idObj);

/**
 * Définis le calque d'un objet
 * \param  context Contexte concerné
 * \param  type    Type de l'objet
 * \param  idObj   Identifiant de l'objet
 * \param  z_index Calque
 * \return         Retourne 1 si succès sinon 0
 */
int SDL_setOnLayer(t_context * context, t_typeData type, int idObj, int z_index);

/**
 * Met à jour le calque d'un objet
 * \param  context Contexte concerné
 * \param  type    Type de donnée
 * \param  idObj   Identifiant de l'objet
 * \param  newId   Nouvel identifiant
 * \param  z_index Calque
 * \return         Retourne 1 si succès sinon 0
 */
int SDL_updateLayer(t_context * context, t_typeData type, int idObj, int newId, int z_index);

/**
 * Supprime un calque
 * \param  context Contexte concerné
 * \param  type    Type de donnée
 * \param  idLayer Identifiant du calque
 * \return         Retourne 1 en cas de succès sinon 0
 */
int SDL_delLayer(t_context * context, t_typeData type, int idObj, int z_index);

/**
* \fn void SDL_generateFrame(t_context * context)
* \brief Construit une frame pour un contexte donnÃ©e et la stocke dans la surface principale en attente de rafraichissement.
*
* \param context Le contexte pour lequel il faut construire une frame
* \return void
*/
void SDL_generateFrame(t_context * context);

/**
 * Met à jour une image
 * \param  context Contexte dans lequel dessiner
 * \param  newZone Zone à mettre à jour
 * \return         Retourne 1 en cas de succès sinon 0
 */
int SDL_updateFrame(t_context * context, SDL_Rect newZone);

/**
* \fn void SDL_UpdateEvents(Input* in)
* \brief Met Ã  jour la table des Ã©vÃ¨nements Ã  un instant t.
*
* \param in Tableau dans lequel les donnÃ©es seront inscrite.
* \return void
*/
void SDL_UpdateEvents(Input* in);

/**
* \fn int SDL_captureInput(t_context * context, int obj)
* \brief Copie le dernier caractÃ¨re capturÃ© dans la bonne destination
*
* \param context Contexte concernÃ©
* \param obj Identifiant de l'objet concernÃ©
* \return bool
*/
int SDL_captureInput(t_context * context, int obj);

/**
* \fn int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y)
* \brief CrÃ©ation d'un nouvelle objet (bouton ou champs de saisie)
*
* \param context Contexte concernÃ©
* \param id Retourne l'identifiant de l'objet crÃ©e mettre Ã  NULL si vous ne souhaitez pas rÃ©cupÃ©rer son identifiant.
* \param type Bouton ou champ de saisie
* \param title Titre de l'objet
* \param align Alignement du texte
* \param dest S'il s'agit d'un champs de saisie, mettre la variable string dans laquelle sera stockÃ© le rÃ©sultat, sinon mettre Ã  NULL.
* \param typeForm S'il s'agit d'un champs de saisie, prÃ©ciser le masque de saisie, sinon mettre NONE.
* \param x Position x relative au contexte de l'objet
* \param y Position y relative au contexte de l'objet
* \return bool
*/
int SDL_newObj(t_context * context, int * id, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
/**
* \fn int SDL_editObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y)
* \brief Modifie les propriÃ©tÃ©s d'un objet
*
* \param context Contexte concernÃ©
* \param obj Identifiant de l'objet Ã  modifier
* \param type Bouton ou champ de saisie
* \param title Titre de l'objet
* \param align Alignement du texte
* \param dest S'il s'agit d'un champs de saisie, mettre la variable string dans laquel sera stockÃ© le rÃ©sultat, sinon mettre Ã  NULL.
* \param typeForm S'il s'agit d'un champs de saisie, prÃ©ciser le masque de saisie, sinon mettre NONE.
* \param x Position x relative au contexte de l'objet
* \param y Position y relative au contexte de l'objet
* \return bool
*/
int SDL_editObj(t_context * context, int obj, t_typeData type, char * title, int align, char * dest, t_typeForm typeForm, int x, int y);
/**
* \fn int SDL_delObj(t_context * context, int obj)
* \brief Supprime d'un contexte un objet
*
* \param context Contexte concernÃ©
* \param obj Identifiant de l'objet Ã  supprimer
* \return bool
*/
int SDL_delObj(t_context * context, int obj);

/**
* \fn int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y)
* \brief CrÃ©er une ligne de texte Ã  afficher pour un contexte
*
* \param context Contexte concernÃ©
* \param id Retourne l'identifiant de l'objet texte crÃ©e mettre Ã  NULL si vous ne souhaitez pas rÃ©cupÃ©rer son identifiant.
* \param content La chaine de caractÃ¨re Ã  associÃ© Ã  cette objet texte.
* \param couleur La couleur du texte
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_newText(t_context * context, int * id, char * content, SDL_Color couleur, int x, int y);
/**
* \fn int SDL_editText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y)
* \brief Modifie une entrÃ©e texte au prÃ©alable chargÃ©e avec SDL_newText()
*
* \param context Contexte concernÃ©
* \param idtext Identifiant de l'objet texte Ã  modifier
* \param content La chaine de caractÃ¨re Ã  associÃ© Ã  cette objet texte.
* \param couleur La couleur du texte
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_editText(t_context * context, int idtext, char * content, SDL_Color couleur, int x, int y);
/**
* \fn int SDL_delText(t_context * context, int idtext)
* \brief Supprime d'un contexte un objet texte
*
* \param context Contexte concernÃ©
* \param idtext Identifiant de l'objet texte Ã  supprimer
* \return bool
*/
int SDL_delText(t_context * context, int idtext);

/**
* \fn int SDL_newImage(t_context * context, int * id, char * file, int x, int y)
* \brief Ajoute une image Ã  un contexte
*
* \param context Contexte concernÃ©
* \param id Retourne l'identifiant de l'image crÃ©e mettre Ã  NULL si vous ne souhaitez pas rÃ©cupÃ©rer son identifiant.
* \param file Le fichier image concernÃ©
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_newImage(t_context * context, int * id, char * file, int x, int y);
/**
* \fn int SDL_editImage(t_context * context, int idimg, int x, int y)
* \brief Modifie les propriÃ©tÃ©s d'une image
*
* \param context Contexte concernÃ©
* \param idimg Identifiant de l'image dont les propriÃ©tÃ©s sont Ã  modifier
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \return bool
*/
int SDL_editImage(t_context * context, int idimg, int x, int y);
/**
* \fn int SDL_delImage(t_context * context, int idimg)
* \brief Supprimer une image d'un contexte
*
* \param context Contexte concernÃ©
* \param idimg Identifiant de l'image Ã  supprimer
* \return bool
*/
int SDL_delImage(t_context * context, int idimg);

/**
* \fn int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide)
* \brief Ajoute un sprite pour un contexte
*
* \param context Contexte concernÃ©
* \param filename Fichier image du sprite (transparence png recommandÃ©)
* \param transparancy Couleur de transparence
* \param sp_height Hauteur d'une partie (souvent le personnage)
* \param sp_width Largeur d'une partie (souvent le personnage)
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \param position Identifiant de la colonne (verticale)
* \param animation Identifiant de la ligne (horizontale)
* \param hide Pour ne pas afficher = 1, 0 pour afficher.
* \return bool
*/
int SDL_newSprite(t_context *context, char * filename, SDL_Color transparancy, int sp_height, int sp_width, int x, int y, int position, int animation, int hide);
/**
* \fn int SDL_editSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide)
* \brief Modifie le sprite d'un contexte donnÃ©e Ã  condition qu'il soit dÃ©jÃ  chargÃ© avec SDL_newSprite()
*
* \param context Contexte concernÃ©
* \param idSprite Identifiant du sprite concernÃ©
* \param x Position x relative au contexte
* \param y Position y relative au contexte
* \param position Identifiant de la colonne (verticale)
* \param animation Identifiant de la ligne (horizontale)
* \param hide Pour ne pas afficher = 1, 0 pour afficher.
* \return bool
*/
int SDL_editSprite(t_context *context, int idSprite, int x, int y, int position, int animation, int hide);
/**
* \fn int SDL_delSprite(t_context *context, int idSprite)
* \brief Supprime un sprite d'un contexte donnÃ©e
*
* \param context Contexte concernÃ©
* \param idSprite Identifiant du sprite Ã  supprimer
* \return bool
*/
int SDL_delSprite(t_context *context, int idSprite);

/**
 * Libère le sprite de la mémoire
*
* \param context Contexte concernÃ©
* \param idSprite Identifiant du sprite Ã  supprimer
*
 */
void SDL_freeSprite(t_context *context, int idSprite);

/**
* \fn int SDL_newRect(t_context *context, int * idrect , SDL_Color color, int height, int width, int x, int y)
* \brief Ajoute un nouveau rectangle pour un contexte donnÃ©e
*
* \param context Contexte concernÃ©
* \param idrect Lier Ã  un entier le nouveau rectangle sinon mettre Ã  NULL
* \param color Couleur du rectangle (SDL_Color)
* \param height Hauteur du nouveau rectangle
* \param width Largeur du nouveau rectangle
* \param x Position x par rapport au contexte
* \param y Position y par rapport au contexte
* \return bool
*/
int SDL_newRect(t_context *context, int * idrect , SDL_Color color, int height, int width, int x, int y);
/**
* \fn int SDL_editRect(t_context *context, int idrect, SDL_Color color, int height, int width, int x, int y)
* \brief Modifie un rectangle existant dans un contexte donnÃ©e
*
* \param context Contexte concernÃ©
* \param idrect Identifiant du rectangle concernÃ©
* \param color Couleur du rectangle (SDL_Color)
* \param height Hauteur du rectangle
* \param width Largeur du rectangle
* \param x Position x par rapport au contexte
* \param y Position y par rapport au contexte
* \return bool
*/
int SDL_editRect(t_context *context, int idrect, SDL_Color color, int height, int width, int x, int y);
/**
* \fn int SDL_delRect(t_context *context, int idrect)
* \brief Supprime un rectangle existant dans un contexte donnÃ©e
*
* \param context Contexte concernÃ©
* \param idrect Identifiant du rectangle concernÃ©
* \return bool
*/
int SDL_delRect(t_context *context, int idrect);

/**
* \fn int SDL_generate(t_context * context)
* \brief GÃ©nÃ¨re une frame et l'affiche Ã  l'Ã©cran, fonction blocante s'il existe des objets (bouton(s) ou champ(s) de saisie).
*
* \param context Le contexte pour lequel il faut gÃ©nÃ©rer une frame
* \return L'identifiant du bouton choisis ou rien.
*/
int SDL_generate(t_context * context);

/**
* \fn int SDL_nbObj(t_context * context)
* \brief Compte le nombre d'objet chargÃ©e en mÃ©moire pour un contexte donnÃ©e (Comprend les bouttons et les champs de saisie)
*
* \param context Le contexte Ã  vÃ©rifier, au prÃ©alable chargÃ© avec SDL_newContext
* \return Nombre d'objet chargÃ©e
*/
int SDL_nbObj(t_context * context);
/**
* \fn int SDL_nbText(t_context * context)
* \brief Compte le nombre d'objet texte chargÃ©e en mÃ©moire pour un contexte donnÃ©e
*
* \param context Le contexte Ã  vÃ©rifier, au prÃ©alable chargÃ© avec SDL_newContext
* \return Nombre d'objet texte chargÃ©e
*/
int SDL_nbText(t_context * context);
/**
* \fn int SDL_nbImage(t_context * context)
* \brief Compte le nombre d'image chargÃ©e en mÃ©moire pour un contexte donnÃ©e
*
* \param context Le contexte Ã  vÃ©rifier, au prÃ©alable chargÃ© avec SDL_newContext
* \return Nombre d'image chargÃ©e
*/
int SDL_nbImage(t_context * context);

/**
* \fn int SDL_nbSprite(t_context * context)
* \brief Compte le nombre de sprite chargÃ©e en mÃ©moire pour un contexte donnÃ©e
*
* \param context Le contexte Ã  vÃ©rifier, au prÃ©alable chargÃ© avec SDL_newContext
* \return Nombre de sprite chargÃ©e
*/
int SDL_nbSprite(t_context * context);

/**
* \fn int SDL_contextEmpty(t_context * context)
* \brief VÃ©rifie si un contexte n'a pas de donnÃ©e chargÃ© en mÃ©moire
*
* \param context Le contexte Ã  vÃ©rifier, au prÃ©alable chargÃ© avec SDL_newContext
* \return Bool
*/
int SDL_contextEmpty(t_context * context);

/**
 * Charge une image au même format que celui de l'écran
 * \param  filePath Chemin de l'image à charger
 * \return          Retourne un pointeur sur l'image chargée sinon NULL
 */
SDL_Surface * SDL_loadImage(char filePath[]);

/**
* \fn t_context * SDL_newContext(char * title, int x, int y, int height, int width)
* \brief CrÃ©ation d'un nouveau contexte
*
* \param title Le titre associÃ© Ã  ce nouveau contexte
* \param x Position x relative Ã  la fenÃªtre SDL
* \param y Position y relative Ã  la fenÃªtre SDL
* \param height Hauteur occupÃ© en pixel par ce nouveau contexte
* \param width Largeur occupÃ© en pixel par ce nouveau contexte
* \return Adresse du nouvelle element t_context *
*/
t_context * SDL_newContext(char * title, int x, int y, int height, int width);

/**
* \fn void SDL_freeContext(t_context * context)
* \brief LibÃ¨re la mÃ©moire, supprime les donnÃ©es chargÃ©s pour un contexte donnÃ©e.
*
* \param context Contexte concernÃ©
* \return void
*/
void SDL_freeContext(t_context * context);

/**
 * Ajoute une couche au contexte
 * @param  context Contexte concerné
 * @return         Retourne 1 si succès sinon 0
 */
int SDL_addSet(t_context * context);

/**
 * Libère la mémoire d'une couche
 * \param context Contexte concerné
 * \param set     Couche à libérer
 */
void SDL_freeSet(t_context * context, int set);

/**
* \fn void SDL_loadRessources()
* \brief Charge en mÃ©moire les ressources de base (Image bouton, champ saisie, son de selection, etc..)
*
* \return void
*/
void SDL_loadRessources();
/**
* \fn void SDL_unloadRessources()
* \brief LibÃ¨re la mÃ©moire des ressources de base, prÃ©cÃ©demment chargÃ©es avec SDL_loadRessources()
*
* \return void
*/
void SDL_unloadRessources();

/**
* \fn int SDL_isKeyPressed(int KEY_S)
* \brief VÃ©rifie si une touche du clavier est enfoncÃ©e.
*
* \param KEY_S Identifiant de la touche (constante SDL_Keycode) (https://wiki.libsdl.org/SDL_Keycode)
* \return Bool
*/
int SDL_isKeyPressed(int KEY_S);

/**
* \fn int SDL_isMousePressed(int MOUSE_S)
* \brief VÃ©rifie si une touche de la souris est enfoncÃ©e.
*
* \param MOUSE_S Identifiant de la touche (constante SDL_Keycode) (https://wiki.libsdl.org/SDL_Keycode)
* \return Bool
*/
int SDL_isMousePressed(int MOUSE_S);

/**
* \fn int SDL_playSound(char * sndfile)
* \brief Lecture d'un fichier son court au prÃ©alable chargÃ© avec SDL_loadSound
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_playSound(const char * sndfile);
/**
* \fn int SDL_loadSound(char * sndfile)
* \brief Chargement en mÃ©moire d'un fichier audio
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_loadSound(const char * sndfile);
/**
* \fn int SDL_unloadSound(char * sndfile)
* \brief LibÃ©rer un fichier audio de la mÃ©moire au prÃ©alable chargÃ© avec SDL_loadSound
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_unloadSound(const char * sndfile);
/**
* \fn int SDL_unloadallSound()
* \brief LibÃ¨re tout les fichiers audio chargÃ© avec SDL_loadSound
*
* \return Bool
*/
int SDL_unloadallSound();

/**
* \fn int SDL_loadMusic(const char * musicfile)
* \brief Prépare un son long de manière à être lu en "streaming"
*
* \param sndfile Fichier audio source (*.wav, *.mp3, *.ogg)
* \return Bool
*/
int SDL_loadMusic(const char * musicfile);

/**
* \fn void SDL_setmaxChannel(unsigned int nbch)
* \brief Modifie le nombre de son joué simultanément
*
* \param nbch Nombre de canaux audio maximal
* \return void
*/
void SDL_setmaxChannel(unsigned int nbch);

/**
* \fn void SDL_setDelaySingleFrame(int delay)
* \brief Le temps de rafraichissement entre chaque frame, par dÃ©fault 50 ms.
*
* \param delay Temps en milliseconde entre chaque frame rendu
* \return void
*/
void SDL_setDelaySingleFrame(unsigned int delay);

/**
* \fn int SDL_requestExit()
* \brief VÃ©rifie si l'utilisateur requiert la fermeture du programme
*
* \return bool
*/
int SDL_requestExit();

/**
* \fn int SDL_getmousex()
* \brief Rend la position x de la souris par rapport Ã  la fenÃªtre
*
* \return Position x (entier)
*/
int SDL_getmousex();
/**
* \fn int SDL_getmousey()
* \brief Rend la position y de la souris par rapport Ã  la fenÃªtre
*
* \return Position y (entier)
*/
int SDL_getmousey();
/**
* \fn int SDL_getimagewidth(t_context * context, int imgid)
* \brief Nous informe de la largeur d'une image depuis un contexte prÃ©cis
*
* \param context Le contexte concernÃ©
* \param imgid Identifiant de l'image
* \return Largeur en pixel (entier)
*/
int SDL_getimagewidth(t_context * context, int imgid);
/**
* \fn int SDL_getimageheight(t_context * context, int imgid)
* \brief Nous informe de la hauteur d'une image depuis un contexte prÃ©cis
*
* \param context Le contexte concernÃ©
* \param imgid Identifiant de l'image
* \return Hauteur en pixel (entier)
*/
int SDL_getimageheight(t_context * context, int imgid);

/**
* \fn int SDL_getposx(t_context * context, int id, t_typeData type)
* \brief Nous informe de la position x d'un objet depuis un contexte prÃ©cis
*
* \param context Le contexte concernÃ©
* \param id Identifiant de l'objet
* \param type Type d'objet, bouton, champs de saisie, image, etc..
* \return Position x, pixel (entier).
*/
int SDL_getposx(t_context * context, int id, t_typeData type);
/**
* \fn int SDL_getposy(t_context * context, int id, t_typeData type)
* \brief Nous informe de la position y d'un objet depuis un contexte prÃ©cis
*
* \param context Le contexte concernÃ©
* \param id Identifiant de l'objet
* \param type Type d'objet, bouton, champs de saisie, image, etc..
* \return Position y, pixel (entier).
*/
int SDL_getposy(t_context * context, int id, t_typeData type);

/**
 * Glisse un objet
 * \param  context Contexte dans lequel glisser
 * \param  typeObj Type de l'objet à glisser
 * \param  idObj   Identifiant de l'objet
 * \return         Retourne 1 en cas de succés, -1 le cas échéant
 */
int SDL_drag(t_context * context, t_typeData typeObj, int idObj);

/**
 * Définis si deux rectangles se chevauchent
 * \param  context Contexte dans lequel dessiner
 * \param  Rect1   Rectangle 1
 * \param  Rect2   Rectangle 2
 * \return         Retourne 2 si Rect 2 dans Rect1, 1 si collision, 0 sinon
 */
int SDL_isInArea(SDL_Rect Rect1, SDL_Rect Rect2);

/**
 * Corrige le clipping
 * @param context Contexte concerné
 * @param clip    Clipping à corriger
 */
void SDL_clamping(SDL_Rect * clip);

/**
 * Récupère le clipping
 * \param  context  Contexte dans lequel dessiner
 * \param  idSprite Identifiant de l'objet
 * \param  clip     Clipping à remplir
 * \return          Retourne 1 en cas de succès sinon 0
 */
int SDL_getClip(t_context * context, t_typeData object, int idObj, SDL_Rect * clip);

/**
 * Dépose un objet
 * \param context Contexte dans lequel déposer
 * \param typeObj Type de l'objet
 * \param idObj   Identifiant de l'objet
 * \param posX    Position X où déposer l'objet
 * \param posY    Position Y où déposer l'objet
 * \return 		  Retourne 1 en cas de succés, -1 le cas échéant
 */
int SDL_drop(t_context * context, t_typeData typeObj, int idObj, int posX, int posY);

/**
* \fn void SDL_setSNDFolder(char * newFolder)
* \brief Change le dossier source pour les sons
*
* \param newFolder Chemin relatif ou absolu ce terminant par /
* \return void
*/
void SDL_setSNDFolder(char * newFolder);
/**
* \fn void SDL_setTTFFolder(char * newFolder)
* \brief Change le dossier source pour les polices d'Ã©critures
*
* \param newFolder Chemin relatif ou absolu ce terminant par /
* \return void
*/
void SDL_setTTFFolder(char * newFolder);
/**
* \fn void SDL_setIMGFolder(char * newFolder)
* \brief Change le dossier source pour les images
*
* \param newFolder Chemin relatif ou absolu ce terminant par /
* \return void
*/
void SDL_setIMGFolder(char * newFolder);

/* esave.c */
/**
* \fn int SDL_saveProfil(d_save * profil)
* \brief Sauvegarde le profil de donnÃ©es dans le fichier
*
* \param profil Profil de donnÃ©es prÃ©cÃ©demment chargÃ© avec initProfil()
* \return bool
*/
int SDL_saveProfil(d_save * profil);
/**
* \fn int SDL_writeParam(d_save * profil, char * param, char * value)
* \brief Sauve un paramÃ¨tre avec sa valeur dans la mÃ©moire (cryptÃ©)
*
* \param profil Profil de donnÃ©es prÃ©cÃ©demment chargÃ© avec initProfil()
* \param param Nom du paramÃ¨tre
* \param value Contenu associÃ© Ã  param en clair.
* \return bool
*/
int SDL_writeParam(d_save * profil, char * param, char * value);
/**
* \fn char * SDL_readParam(d_save * profil, char * param)
* \brief RÃ©cupÃ¨re le contenu associÃ© au paramÃ¨tre
*
* \param profil Profil de donnÃ©es prÃ©cÃ©demment chargÃ© avec initProfil()
* \param param Nom du paramÃ¨tre
* \return bool
*/
char * SDL_readParam(d_save * profil, char * param);
/**
* \fn d_save * SDL_initProfil(char * filename)
* \brief Initialise le profil, obligatoire pour manipuler un profil de donnÃ©e
*
* \param filename Fichier sauvegarde Ã  manipuler
* \return Pointeur vers le jeu de donnÃ©e d_save*
*/
d_save * SDL_initProfil(char * filename);
/**
* \fn void SDL_freeProfil(d_save * profil)
* \brief LibÃ¨re le profil
*
* \param profil Profil de donnÃ©es prÃ©cÃ©demment chargÃ© avec initProfil()
* \return void
*/
void SDL_freeProfil(d_save * profil);

/* db_lite.c */
extern sqlite3 *db;

int db_open(char * filename);
void db_close();
int writeBlob(sqlite3 *db, const char *zKey, const unsigned char *zBlob, int nBlob);
int readBlob(sqlite3 *db, const char *zKey, unsigned char **pzBlob, int *pnBlob);
int readText(sqlite3 *db, int zID, char **pzBlob, int *pnBlob);
int dropBlobTable();
int createBlobTable();

/* cstring.c */
long getcharocc(char * text, char elem);
void replaceinstring(char * text, char elem, char newc);
void unsignedchar_memcpy(unsigned char *dest, unsigned char *src, size_t len);
void formatedcpy(char *dst, char *src, size_t srclen);

/* aes.c */
extern EVP_CIPHER_CTX en, de;
extern unsigned int salt[];
extern const unsigned char key_data[];
extern const int key_data_len;

int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, EVP_CIPHER_CTX *d_ctx);
unsigned char *aes_decrypt(EVP_CIPHER_CTX *e, unsigned char *ciphertext, int *len);
unsigned char *aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
int aes_custom_32key(const unsigned char *key32);

//extern unsigned int DELAY_EACH_FRAME;

extern SDL_Surface *screen;
extern SDL_Color colorRed, colorWhite, colorBlack, colorGreenLight;
/** \brief Contient la suite des Ã©vÃ©nements capturÃ©s par la SDL (Clavier+Souris) */
extern SDL_Event GlobalEvent;

/** \brief Identifiant du bouton survolÃ© */
extern char buffer; //Where we will keep last char from keyboard !
extern int buffer_deliver;
extern Input in;