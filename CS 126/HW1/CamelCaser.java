/**
 * A Class that implements camel casing a given input if the input is valid  
 */
public class CamelCaser {
    /**
     *  Returns camel cased putput for a given input if valid
     * @param inputString input value
     * @return returnStr camel cased return value
     */
    public static String camelCase(String inputString) {
        /*
            A series of preliminary tests to weed out bad arguments
         */
        if(inputString == null){
            throw new IllegalArgumentException(ErrorConstants.NULL_INPUT);
        }

        if(inputString.length() == 0) {         // testing for an empty string and throwing invalid character exception
            return "";
        }
        /*
            testing for valid characters in the argument, throwing an exception if invalid characters present.
         */
        if(!(inputString.matches("[a-z A-Z0-9]+"))) {
            throw new IllegalArgumentException(ErrorConstants.INVALID_CHARACTER);
        }
        /*
            if preliminary tests were passed we declare the variables needed for working
         */
        String[] splitInp = inputString.toLowerCase().split("[ ]+"); // splitting words based on spaces,
                                                                         // could lead to a bug if 2 spaces at once
        if (splitInp.length == 0) {
            return "";
        }

        /*
            checking to see if none of the words start with a digit, throwing an exception if not
         */
        for (String aSplitInp : splitInp) {
            if (aSplitInp.length() != 0 && Character.isDigit(aSplitInp.charAt(0))) {
                throw new IllegalArgumentException(ErrorConstants.INVALID_FORMAT);
            }
        }

        /*
            implementing the algorithm if all the parameters are satisfied
            also finding the first instance of a valid word in case there are multiple whitespaces in the beginning
         */
        int firstWordIndex = 0;

        for(int i = 0; i < splitInp.length; i++) {
            if(splitInp[i].length() != 0){
                firstWordIndex = i;             //first valid word found by iterating through the word array
                break;
            }
        }

        StringBuilder returnStr = new StringBuilder(splitInp[firstWordIndex]);  //using string builder at the suggestion of
                                                                                //the compiler as += concat copies the string everytime.
        for(int j = firstWordIndex+1; j < splitInp.length; j++) {
            // append only the strings which are not null
            returnStr.append(splitInp[j].substring(0, 1).toUpperCase()).append(splitInp[j].substring(1));
        }

        return returnStr.toString();
    }
}
