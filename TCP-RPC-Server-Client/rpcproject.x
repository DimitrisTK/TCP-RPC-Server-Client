/* Kourias Triantafyllos-Dimitrios cs141092 */

/* Contains the array and his size given by the user */
struct User_array
{
	int userarray <100>;
	int userarray_size;
};

/* Contains the result of max min values of the array
 which later assigned on an array with two cells */
struct max_min
{
	int max;
	int min;
};

/* This struct is used for the third option,contains the array and his size used for the multiplication and the number to multiply with */
struct multiplication
{
	int userarray <100>;
	int userarray_size;
	float multiplier;
};

/* The array after the multiplication with the number given by the user */
struct product_array
{
	float prodarr <100>;
};

program AVERAGE_PROG
{
	version AVERAGE_VERS
	{
		float average (User_array) = 1;
	} = 1;
} = 0x23451111;


program MAXMIN_PROG
{
	version MAXMIN_VERS
	{
		max_min maxmin(User_array) = 2;
	} = 1;
} = 0x23451112;


program PRODUCT_PROG
{
	version PRODUCT_VERS
	{
		product_array product(multiplication) = 3;
	} = 1;
} = 0x23451113;
