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

	public static ExecutorService getExecutorService() {
		if (executor == null) {
			int numProcessors = Runtime.getRuntime().availableProcessors();
			executor = Executors.newFixedThreadPool(numProcessors);
		}
		return executor;
	}

	public static void execute(Runnable r) {
		getExecutorService().execute(r);
	}

	public static void shutDownExecutor() {
		getExecutorService().shutdown();
	}
}
