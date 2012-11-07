package cleanerversion;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * A singleton(kinda) that provides an executor service. The application  
 * should only ever have one executor service.
 * This class also provides easy accessors to methods in the executor for ease 
 * of use. 
 */
public class ExecutorServiceProvider {
	private static ExecutorService executor = null;
	private static int numThreads = Runtime.getRuntime().availableProcessors();

	public static ExecutorService getExecutorService() {
		if (executor == null) {
			executor = Executors.newFixedThreadPool(getNumConcurrentThreads());
		}
		return executor;
	}

	public static void setNumConcurrentThreads(int num) {
		numThreads = num;
	}

	public static int getNumConcurrentThreads() {
		return numThreads;
	}

	public static void execute(Runnable r) {
		getExecutorService().execute(r);
	}

	public static void shutDownExecutor() {
		getExecutorService().shutdownNow();
	}
}
