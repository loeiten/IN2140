/*
 * Assignment:
 *
 * `printf()` is often used to
 * - Write a message to the user
 * - Write debug information
 * - Write error messages
 *
 * The problem with this is that it's often hard to separate the three.
 * A standard way to solve the problem is to have a system for telling something
 * about *how important* the message is in addition to the message itself.
 *
 * Such system could look like
 *
 *     log_msg(DEBUG, "The value of a is 10.");
 *     log_msg(ERROR, "Something is wrong here!");
 *     log_msg(INFO, "Hello world :)");
 *
 * A
 * Implement a system similar to the one shown above
 *
 * B
 * Add support for selecting the level to be printed through command line
 * arguments.
 * You could use `getopt()`, but it's not required
 */
