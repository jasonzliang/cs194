package cleanerversion;

import java.io.IOException;

public interface PrintableElementContainer {
	/**
	 * @param fileName the file name to write to without an extension
	 */
	public void printToMatrixMarketFile(String fileName) throws IOException;

	/**
	 * @param fileName the file name to write to without an extension
	 * @param decrementCount adjust the count at the top of rows to 1 less than it actually is. Useful if indexing from 1.
	 */
	public void printToMatrixMarketFile(String fileName, boolean decrementCount) throws IOException;
}
