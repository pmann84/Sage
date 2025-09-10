#pragma once

#include "sage/string/utilities.hpp"

#include <any>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <algorithm>
#include <sstream>
#include <functional>
#include <filesystem>

namespace sage::argparse
{
    namespace exceptions
    {
        class unknown_argument : public std::exception
        {
        public:
            explicit unknown_argument(const std::string& unknown_argument_name)
            {
                std::stringstream error_msg;
                error_msg << "Error: Attempt to Access Unknown Argument: " << unknown_argument_name << std::endl;
                m_error_message = error_msg.str();
            }

            [[nodiscard]] char const* what() const noexcept override
            {
                return m_error_message.c_str();
            }
        private:
            std::string m_error_message;
        };

        class invalid_narg_mode : public std::exception
        {
        public:
            explicit invalid_narg_mode(const std::string& narg_mode)
            {
                std::stringstream error_msg;
                error_msg << "Error: Invalid NARGs specification: " << narg_mode << std::endl;
                m_error_message = error_msg.str();
            }

            [[nodiscard]] char const* what() const noexcept override
            {
                return m_error_message.c_str();
            }
        private:
            std::string m_error_message;
        };
    }

    namespace validate
    {
        inline bool is_optional(const std::string& argument_name)
        {
            return string::utilities::starts_with(argument_name, std::string("-"));
        }

        inline bool is_valid_argument_flags(const std::vector<std::string>& argument_names)
        {
            bool positional = false;
            bool optional = false;
            for (auto& name : argument_names)
            {
                if (is_optional(name))
                {
                    optional = true;
                }
                else
                {
                    positional = true;
                }
            }
            if (optional && positional) return false;
            return true;
        }
    }

    namespace utils
    {
        // Primary template that supports types that aren't containers
        template<typename T, typename = void>
        struct is_container : std::false_type {};

        // Specialisation that says strings are not classed as containers here
        template<>
        struct is_container<std::string> : std::false_type {};

        // Specialisation that recognises types that are containers
        template<typename T>
        struct is_container<T,
                std::void_t<typename T::value_type,
                decltype(std::declval<T>().begin()),
                decltype(std::declval<T>().end())>
        > : std::true_type {};

    // Convenience for testing types
    template <typename T>
    static constexpr bool is_container_value = is_container<T>::value;
}

    enum class NargsMode
    {
        Integer, // N - consumes N arguments into a list
        Single, // ? - consumes one arg if possible and produces a single item - if no arg specified then default value will be used
        All, // * - all args are gathered into a list.
        AtLeastOne, // + - (like *) all args are gathered into a list. Error message generated if there wasn't at least one arg present
    };

    // Set of classes to read config files
    class i_config_file_reader
    {
    public:

    };

    // Expand support to include json, xml
    // For now we just support : delimited values with each on a separate line
    class config_file_reader : public i_config_file_reader
    {

    };

    class argument
    {
    public:
        explicit argument(const std::vector<std::string>& names) : m_flags(names), m_nargs(1), m_nargs_mode(NargsMode::Integer)
        {
            // Validate that if one name starts with - then all must!
            if (!validate::is_valid_argument_flags(m_flags))
            {
                throw std::runtime_error(
                        "Error: Invalid option string: all names must start with a character '-' or none of them.");
            }
            // Use the longest name as the destination
            std::string longest_arg = string::utilities::get_string_with_max_size(m_flags);
            m_destination = longest_arg;
            if (validate::is_optional(longest_arg))
            {
                m_destination = string::utilities::trim_left(longest_arg, '-');
            }

        }

        explicit argument(const std::string& name) : argument(std::vector<std::string>(1, name))
        {
        }

        template<typename ArgT>
        void value(ArgT value)
        {
            if (m_nargs_mode == NargsMode::All || m_values.size() < m_nargs)
            {
                m_values.push_back(value);
                return;
            }
            std::stringstream ss;
            ss << "Error: Attempt to store more than " << m_nargs << " values in argument " << m_destination << ".";
            throw std::runtime_error(ss.str());
        }

        template <typename T>
        static T any_container_cast(const std::vector<std::any>& container)
        {
            using value_t = typename T::value_type;
            T result;
            std::transform(container.begin(), container.end(), std::back_inserter(result),
                           [](const auto& c_value)
                           {
                               return std::any_cast<value_t>(c_value);
                           }
            );
            return result;
        }

        template<typename ReturnArgT>
        ReturnArgT get() const
        {
            if (!m_values.empty())
            {
                if (utils::is_container_value<ReturnArgT>)
                {
                    // Return all the values
                    return any_container_cast<ReturnArgT>(m_values);
                }
                else
                {
                    return std::any_cast<ReturnArgT>(m_values.front());
                }
            }
            if (m_default_value.has_value())
            {
                if (utils::is_container_value<ReturnArgT>)
                {
                    // Return all the values
                    return any_container_cast<ReturnArgT>({m_default_value});
                }
                else
                {
                    return std::any_cast<ReturnArgT>(m_default_value);
                }
            }
            if (m_nargs_mode == NargsMode::All || m_nargs_mode == NargsMode::Single)
            {
                return ReturnArgT();
            }
            throw std::runtime_error("No value provided for argument.");
        }

        // The number of command-line arguments that should be consumed.
        argument& num_args(size_t n)
        {
            m_nargs_mode = NargsMode::Integer;
            m_nargs = n;
            return *this;
        }

        argument& num_args(const std::string& n)
        {
            if (n == "?")
            {
                m_nargs_mode = NargsMode::Single;
            }
            else if (n == "*")
            {
                m_nargs_mode = NargsMode::All;
            }
            else if (n == "+")
            {
                m_nargs_mode = NargsMode::AtLeastOne;
            }
            else
            {
                throw exceptions::invalid_narg_mode(n);
            }
            return *this;
        }

        [[nodiscard]] size_t num_args() const
        {
            return m_nargs;
        }

        [[nodiscard]] NargsMode num_args_mode() const
        {
            return m_nargs_mode;
        }

        // The value produced if the argument is absent from the command line and if it is absent from the namespace object.
        // This works for all optional arguments but only works
        // for positional args set to Single (?) or All (*)
        template<typename ArgT>
        argument& default_value(ArgT value)
        {
            m_default_value = value;
            return *this;
        }

        argument& help(const std::string& help)
        {
            m_help = help;
            return *this;
        }

        [[nodiscard]] std::string help() const
        {
            return m_help;
        }

        argument& dest(const std::string& dest)
        {
            m_destination = dest;
            return *this;
        }

        [[nodiscard]] std::string dest() const
        {
            return m_destination;
        }

        [[nodiscard]] bool is_optional() const
        {
            return validate::is_optional(m_flags[0]);
        }

    //        friend std::ostream &operator<<(std::ostream &os, const argument &arg);

        [[nodiscard]] std::string get_name_string() const
        {
            return string::utilities::join(m_flags, std::string(", "));
        }

        [[nodiscard]] std::string get_longest_name_string() const
        {
            return string::utilities::get_string_with_max_size(m_flags);
        }

        [[nodiscard]] bool matches_arg_name(const std::string& arg_name) const
        {
            return std::find_if(
                    m_flags.begin(),
                    m_flags.end(),
                    [&arg_name](const std::string& flag)
                    {
                        return flag == arg_name;
                    }
            ) != m_flags.end();
        }

        [[nodiscard]] bool is_set() const noexcept
        {
            if (m_nargs_mode == NargsMode::All || m_nargs_mode == NargsMode::Single || m_default_value.has_value())
            {
                return true;
            }
            if (m_values.size() < m_nargs)
            {
                return false;
            }
            bool is_set = true;
            for (auto& val : m_values)
            {
                if (!val.has_value())
                {
                    is_set = false;
                }
            }
            return is_set;
        }

    private:
        std::vector<std::any> m_values;
        std::any m_default_value;
        std::string m_destination;
        std::vector<std::string> m_flags;
        std::string m_help;
        size_t m_nargs;
        NargsMode m_nargs_mode;
    };

    class argument_parser
    {
    public:
        argument_parser() : argument_parser("", "")
        {
        }

        argument_parser(std::string program_name, std::string description)
                : m_program_name(std::move(program_name))
                , m_description(std::move(description))
                , m_config_file_mode(false)
                , m_environment_mode(false)
        {
            add_argument({"-h", "--help"})
                    .num_args(0)
                    .help("Show this help message and exit.");
        }

        argument_parser& description(const std::string &description)
        {
            m_description = description;
            return *this;
        }

        argument_parser& name(const std::string& name)
        {
            m_program_name = name;
            return *this;
        }

        // Do we want to support other methods of config input
        // - Command line - current method
        // - Config file - specify config file path on command line
        // - Environment - specify prefix to read the environment variables from
        // These should probably cascade so that one can have multiple enabled ala .NET host builder
        argument_parser& enable_config_file()
        {
            m_config_file_mode = true;
            add_argument({"-c", "--configFile"})
                    .num_args(1)
                    .help("Read in arguments from the specified config file.");
            return *this;
        }

        argument_parser& enable_environment_config(const std::string& prefix)
        {
            m_environment_mode = true;
            m_environment_prefix = prefix;
            return *this;
        }

        argument_parser& enable_environment_config()
        {
            return enable_environment_config("APP_ENV_");
        }

        argument& add_argument(const std::initializer_list<std::string> names)
        {
            argument arg(names);
            return add_argument(arg);
        }

        argument& add_argument(const std::string &name)
        {
            argument arg(name);
            return add_argument(arg);
        }

        template <typename ArgT>
        ArgT get(std::string name)
        {
            auto pos_it = std::find_if(
                    m_positional_arguments.begin(),
                    m_positional_arguments.end(),
                    [&name](const argument& arg) -> bool
                    {
                        return arg.dest() == name;
                    }
            );
            auto opt_it = std::find_if(
                    m_optional_arguments.begin(),
                    m_optional_arguments.end(),
                    [&name](const argument& arg) -> bool
                    {
                        return arg.dest() == name;
                    }
            );

            if (pos_it != m_positional_arguments.end())
            {
                return pos_it-> template get<ArgT>();
            }

            if (opt_it != m_optional_arguments.end())
            {
                return opt_it->template get<ArgT>();
            }
            throw exceptions::unknown_argument(name);
        }

        template <typename ArgT>
        bool try_get(const std::string& name, ArgT& result)
        {
            try
            {
                const auto argValue = get<ArgT>(name);
                result = argValue;
                return true;
            }
            catch (std::runtime_error& e)
            {
                return false;
            }
        }

        void parse_args(int argc, char *argv[])
        {
            // Get the input from the command line
            std::vector<std::string> command_line_args;
            std::copy(argv, argv + argc, std::back_inserter(command_line_args));

            // Get values from environment
    //            std::string getEnvVar( std::string const & key ) const
    //            {
    //                char * val = getenv( key.c_str() );
    //                return val == NULL ? std::string("") : std::string(val);
    //            }
            // Get values from config file

            // Check if we have a name, if not, then take it from the arguments list
            if (m_program_name.empty() && !command_line_args.empty())
            {
                m_program_name = std::filesystem::path(command_line_args.front()).filename().string();
            }

            // Iterate over the given arguments
            size_t pos_index = 0;
            for (auto it = command_line_args.begin() + 1; it < command_line_args.end(); ++it)
            {
                // Check if it's positional or not
                if (validate::is_optional(*it))
                {
                    process_optional_command_line_argument(command_line_args, it);
                }
                else
                {
                    process_positional_command_line_argument(pos_index, command_line_args, it);
                }
            }

            // Now we need to check that all positional arguments have been filled
            check_for_missing_arguments();
        }

    private:
        argument& add_argument(const argument& arg)
        {
            if (arg.is_optional())
            {
                m_optional_arguments.push_back(arg);
                return m_optional_arguments.back();
            }
            m_positional_arguments.push_back(arg);
            return m_positional_arguments.back();
        }

        argument& get_help_argument()
        {
            return m_optional_arguments[0];
        }

        void consume_n_args(
                argument& arg,
                std::vector<std::string>& command_line_args,
                std::vector<std::string>::iterator& current_arg)
        {
            // Integer, N - consumes N arguments into a list
            auto count = arg.num_args();
            while (count != 0)
            {
                // check each arg is not another flag, or we've reached the end (not enough args)
                if (current_arg == command_line_args.end() || validate::is_optional(*current_arg))
                {
                    std::stringstream ss;
                    ss << "Error: Insufficient positional arguments. " << arg.dest() << " expected " << count << " more input(s) (" << arg.num_args() << " total)." << std::endl;
                    print_error_usage_and_exit(ss.str());
                }
                else
                {
                    // Grab the value and stick it in the next positional argument
                    arg.value(*current_arg);
                }
                // Advance to the next arg unless it's the last one
                if (count > 1)
                {
                    ++current_arg;
                }
                --count;
            }
        }

        static void consume_all_args(
                argument& arg,
                std::vector<std::string>& command_line_args,
                std::vector<std::string>::iterator& current_arg)
        {
            // All - * - all args are gathered into a list.
            bool still_consuming = true;
            while (still_consuming)
            {
                if ((current_arg+1 == command_line_args.end())
                    || (!validate::is_optional(*current_arg) && validate::is_optional(*(current_arg+1))))
                {
                    arg.value(*current_arg);
                    still_consuming = false;
                    break;
                }
                else
                {
                    arg.value(*current_arg);
                    ++current_arg;
                }
            }
        }

        void consume_at_least_one_arg(
                argument& arg,
                std::vector<std::string>& command_line_args,
                std::vector<std::string>::iterator& current_arg)
        {
            // AtLeastOne - + - (like *) all args are gathered into a list. Error message generated if there wasn't at least one arg present

            // check each arg is not another flag, or we've reached the end (not enough args)
            if (current_arg == command_line_args.end() || validate::is_optional(*current_arg))
            {
                std::stringstream ss;
                ss << "Error: Insufficient positional arguments. " << arg.dest() << " expected one or more (+) input(s)." << std::endl;
                print_error_usage_and_exit(ss.str());
            }

            bool still_consuming = true;
            while (still_consuming)
            {
                if ((current_arg+1 == command_line_args.end())
                    || (!validate::is_optional(*current_arg) && validate::is_optional(*(current_arg+1))))
                {
                    arg.value(*current_arg);
                    still_consuming = false;
                    break;
                }
                else
                {
                    arg.value(*current_arg);
                    ++current_arg;
                }
            }
        }

        static void consume_single_arg(
                argument& arg,
                std::vector<std::string>& command_line_args,
                std::vector<std::string>::iterator& current_arg)
        {
            // Single - ? - consumes one arg if possible and produces a single item - if no arg specified then default value will be used
            // Check next arg is not another flag or if we've reached the end (not enough args)
            if (current_arg == command_line_args.end() || validate::is_optional(*current_arg))
            {
                // pass
            }
            else
            {
                // Grab the value and stick it in the next positional argument
                arg.value(*current_arg);
            }
        }

        void check_for_missing_arguments()
        {
            std::vector<std::string> missing_arguments;
            for (const auto& pos_arg : m_positional_arguments)
            {
                if (!pos_arg.is_set())
                {
                    missing_arguments.push_back(pos_arg.dest());
                }
            }
            if (!missing_arguments.empty())
            {
                std::stringstream ss;
                ss << "Error: The following arguments are required: ";
                for (auto& arg : missing_arguments)
                {
                    ss << arg << " ";
                }
                ss << std::endl;
                print_error_usage_and_exit(ss.str());
            }
        }

        void process_optional_command_line_argument(std::vector<std::string>& command_line_args, std::vector<std::string>::iterator& it)
        {
            // If asked for help then print usage and help and exit
            if (get_help_argument().matches_arg_name(*it))
            {
                std::cout << get_usage_and_help_string();
                std::exit(0);
            }
            // Search optional args for the name, increment the iterator and grab the value
            // if the next value is another optional flag (i.e. no value has been specified)
            // and nargs is > 0 then ERROR
            auto arg_it = std::find_if(
                    m_optional_arguments.begin(),
                    m_optional_arguments.end(),
                    [&it](const argument& arg)
                    {
                        return arg.matches_arg_name(*it);
                    }
            );
            if (arg_it == m_optional_arguments.end())
            {
                std::stringstream ss;
                ss << "Error: Unknown optional argument. " << *it << std::endl;
                print_error_usage_and_exit(ss.str());
            }
            else
            {
                auto& opt_arg = *arg_it;
                ++it;
                switch (opt_arg.num_args_mode())
                {
                    case NargsMode::Integer:
                        consume_n_args(opt_arg, command_line_args, it);
                        break;
                    case NargsMode::All:
                        consume_all_args(opt_arg, command_line_args, it);
                        break;
                    case NargsMode::AtLeastOne:
                        consume_at_least_one_arg(opt_arg, command_line_args, it);
                        break;
                    case NargsMode::Single:
                        consume_single_arg(opt_arg, command_line_args, it);
                        break;
                }
                // // Get the number of args meant to be consumed
                // if (arg_it->num_args() > 0)
                // {
                //     // Get the next num_arg arguments
                //     auto count = arg_it->num_args();
                //     while (count != 0)
                //     {
                //         // Get the next arg
                //         it++;
                //         // check each is not another flag, or we haven't hit the end
                //         if (it == command_line_args.end() || validate::is_optional(*it))
                //         {
                //             std::stringstream ss;
                //             ss << "Error: Insufficient optional arguments. " << arg_it->dest() << " expected " << count << " more input(s) (" << arg_it->num_args() << " total)." << std::endl;
                //             print_error_usage_and_exit(ss.str());
                //         }
                //         else
                //         {
                //             arg_it->value(*it);
                //         }
                //         --count;
                //     }
                // }
                // else
                // {
                //     arg_it->value(true);
                // }
            }
        }

        void process_positional_command_line_argument(size_t& pos_arg_index, std::vector<std::string>& command_line_args, std::vector<std::string>::iterator& current_arg)
        {
            // Verify we have enough positional arguments
            if (pos_arg_index >= m_positional_arguments.size())
            {
                std::stringstream ss;
                ss << "Error: Too many positional arguments." << std::endl;
                print_error_usage_and_exit(ss.str());
            }
            // Consume the required number of arguments
            // Get the next num_arg arguments
            argument& pos_arg = m_positional_arguments[pos_arg_index];
            switch (pos_arg.num_args_mode())
            {
                case NargsMode::Integer:
                    consume_n_args(pos_arg, command_line_args, current_arg);
                    break;
                case NargsMode::All:
                    consume_all_args(pos_arg, command_line_args, current_arg);
                    break;
                case NargsMode::AtLeastOne:
                    consume_at_least_one_arg(pos_arg, command_line_args, current_arg);
                    break;
                case NargsMode::Single:
                    consume_single_arg(pos_arg, command_line_args, current_arg);
                    break;
            }
            pos_arg_index++;
        }

        [[nodiscard]] std::string get_usage_string() const
        {
            // Optional arguments are surrounded by [], with the name being the longest flag available
            // Positional arguments are printed as the dest name
            std::stringstream ss;
            ss << "Usage: " << m_program_name << " ";
            for (auto& arg : m_optional_arguments)
            {
                // If single -- name show that, if only - name, show that arg.
                std::string longest_arg_name = arg.get_longest_name_string();
                std::string arg_output_str = sage::string::utilities::to_upper(sage::string::utilities::trim_left(longest_arg_name, '-'));
                ss << "[" << longest_arg_name;
                size_t count = arg.num_args();
                while (count > 0)
                {
                    ss << " " << arg_output_str;
                    --count;
                }
                ss << "] ";
            }
            for (auto& arg : m_positional_arguments)
            {
                switch (arg.num_args_mode())
                {
                    case NargsMode::Integer:
                    {
                        size_t count = arg.num_args();
                        while (count > 0)
                        {
                            // Output the dest name
                            ss << arg.dest() << " ";
                            --count;
                        }
                        break;
                    }
                    case NargsMode::All:
                    {
                        ss << "[" << arg.dest() << " [" << sage::string::utilities::to_upper(arg.dest()) << " ...]] ";
                        break;
                    }
                }

            }
            return ss.str();
        }

        [[nodiscard]] std::string get_help_string() const
        {
            std::stringstream ss;
            ss << std::endl;
            if (!m_description.empty())
            {
                ss << "\t" << m_description << std::endl << std::endl;
            }

            if (!m_positional_arguments.empty())
            {
                ss << "Positional Arguments: " << std::endl;
                for (auto& pos : m_positional_arguments)
                {
                    ss << pos.get_name_string() << ": " << pos.help() << std::endl;
                }
                ss << std::endl;
            }

            if (!m_optional_arguments.empty())
            {
                ss << "Optional Arguments: " << std::endl;
                for (auto& opt : m_optional_arguments)
                {
                    ss << opt.get_name_string() << ": " << opt.help() << std::endl;
                }
                ss << std::endl;
            }
            return ss.str();
        }

        [[nodiscard]] std::string get_usage_and_help_string() const
        {
            std::stringstream ss;
            ss << get_usage_string() << std::endl;
            ss << get_help_string() << std::endl;
            return ss.str();
        }

        void print_error_usage_and_exit(const std::string& message) const
        {
            std::cout << message << std::endl;
            std::cout << get_usage_string() << std::endl;
            std::exit(1);
        }

    private:
        std::string m_program_name;
        std::string m_description;
        std::vector<argument> m_positional_arguments;
        std::vector<argument> m_optional_arguments;
        bool m_config_file_mode;
        bool m_environment_mode;
        std::string m_environment_prefix;
    };
}