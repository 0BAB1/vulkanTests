#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//Set vulkan SDK validation layers
const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public:
    void run() {
        //Run routine
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    // Attributes
    GLFWwindow* window;                                 //GLFW Window handler
    VkInstance instance;                                //VULKAN Instance handler

    void initWindow(){
        //window init routine
        glfwInit();                                     // Init GLFW

        //Windows hints for creation
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);   // Prevent default openGl context
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);     // Prevent RESZABLE

        window = glfwCreateWindow(WIDTH,                //Create the window and assign it to the app's handler
                                  HEIGHT,
                                  "Vulkan",
                                  nullptr,
                                  nullptr);
    }

    void initVulkan() {
        //Vulkan init routine
        createInstance();                               //Call vulkan instance creator
    }

    void createInstance(){
        // Check validation layers support if enabled
        /*
         * So basically, validation layers are these checks
         * You manually add as Vulkan and drivers will not do
         * This work for you
         *
         */
        if(enableValidationLayers && !checkValidationLayerSupport()){
            throw std::runtime_error("Validations layers activated but not at disposition!");
        }

        VkApplicationInfo appInfo{};                                                // Create appInfos object (optional)
        appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;           // As usual, struct type
        appInfo.pApplicationName    = "Test Triangle";
        appInfo.applicationVersion  = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName         = "No Engine";
        appInfo.engineVersion       = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion          = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};                                          // Create info object to create instance
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;                  // As usual, struct type
        createInfo.pApplicationInfo = &appInfo;                                     // Give it our bonus infos

        // GLFW Extension support & checks
        uint32_t glfwExtensionCount = 0;                                            // Basic counter
        const char** glfwExtensions;                                                // String array to hold extension infos
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);    // assign infos and pass it the counter

        //add extensions to create Info
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
            throw std::runtime_error("Echec de la création de l'instance !");
        }


        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Extensions :\n";

        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
    }

    bool checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for(const char* layerName : validationLayers){
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers){
                if(!strcmp(layerName, layerProperties.layerName) == 0){
                    return false;
                }
            }
            return true;
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
};

int main() {
    HelloTriangleApplication app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
